/*
 * Copyright (C) 2014  Roel Janssen <roel@moefel.org>
 *
 * This file is part of moefel.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <sqlite3.h>
#include <zip.h>
#include <time.h>
#include <unistd.h>

#include "parsers/repository.h"
#include "network/http.h"
#include "database/installation.h"
#include "database/repositories.h"
#include "database/packages.h"
#include "packagers/zip.h"
#include "misc/datetime.h"
#include "misc/strings.h"

#define DATABASE_NAME "moefel.db"

/*----------------------------------------------------------------------------.
 | SHOW_HELP                                                                  |
 | This function displays the possible command-line arguments.                |
 '----------------------------------------------------------------------------*/
static void
show_help ()
{
  puts ("\nThe most commonly used commands are:\n"
	"    enable     Add a repository.\n"
	"    disable    Remove a repository.\n"
//	"    update     Update metadata of repositories.\n"
	"    create     Create a new package.\n"
	"    extract    Extract a local package.\n"
	"    list       List packages.\n"
	"    search     Search for packages.\n"
	"    get        Retrieve a package.\n"
//	"    set        Configure default behvior of this program.\n"
	"    version    Show versioning information.\n"
	"    help       Show this message.\n\n"
	"by passing --help after any of these commands, you'll\n"
	"get detailed info (if available).\n");
}

/*----------------------------------------------------------------------------.
 | SHOW_VERSION                                                               |
 | This function (implemented as a macro) displays the current version.       |
 '----------------------------------------------------------------------------*/
#define show_version() printf ("Moefel v%s\n\nModules:\nlibcurl: %s\nlibzip: "\
  "%s\nSQLite: %s\n\nCopyright (C) 2014 Roel Janssen <roel@moefel.org>.\nThis"\
  " is free software; see the source for copying conditions.  There is NO\n"\
  "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR "\
  "PURPOSE.\n\n", VERSION, curl_version_info(CURLVERSION_NOW)->version, \
  LIBZIP_VERSION, sqlite3_libversion())

/*----------------------------------------------------------------------------.
 | SHOW_USAGE                                                                 |
 | This function (implemented as a macro) displays a usage message.           |
 | It will only work when called from within main().                          |
 '----------------------------------------------------------------------------*/
#define show_usage(m) { printf ("Usage:\n  %s %s\n", argv[0], m); return 1; }

/*----------------------------------------------------------------------------.
 | SHOW_PACKAGE_LIST                                                          |
 | This function displays output of a package overview.                       |
 '----------------------------------------------------------------------------*/
static void show_package_list (dt_list* packages)
{
  if (packages != NULL)
    puts ("NAME                          REPOSITORY       LICENSE     CREATED_AT\n"
	  "----------------------------- ---------------- ----------- ----------");

  dt_list* packages_head = packages;
  while (packages != NULL)
    {
      dt_package* p = (dt_package*)packages->data;

      /* Make sure only the date is shown to save some space. */
      p->created_at[10] = '\0';

      printf ("%-29s %-16s %-11s %s\n",
	      p->name,
	      p->domain,
	      p->license,
	      p->created_at);

      dt_package_free (p), packages->data = NULL;
      packages = packages->next;
    }

  dt_list_free (packages_head);
}

/*----------------------------------------------------------------------------.
 | MAIN                                                                       |
 | Execution of the program starts here.                                      |
 '----------------------------------------------------------------------------*/
int
main (int argc, char** argv)
{
  /* At least one argument should be passed to the program, otherwise it has
   * nothing to do. This is probably not the intended use of the program, so
   * let's display the help message. */
  if (argc < 2 || !strcmp (argv[1], "help"))
    {
      show_help ();
      return 0;
    }

  /* Set up the database before we could possibly need its functionality. 
   * This function won't do anything when the file already exists. */
  db_setup (DATABASE_NAME);

  /* OPTION: enable
   * ----------------------------------------------------------------------
   * This option gives the user the ability to add a repository.
   */
  if (!strcmp (argv[1], "enable"))
    {
      /* When the user hasn't provided enough arguments, show a 
       * usage message hinting on how to use this command. This macro
       * will prevent the rest of the code from executing. */
      if (argc <= 2 || !strcmp (argv[2], "--help"))
	show_usage ("enable <domain.tld>");

      /* These variables a purely there for a clearer understanding
       * of the code that follows. */
      const char* domain = argv[2];

      curl_global_init (CURL_GLOBAL_ALL);

      /* Get the name and packages from the remote host. */
      dt_http_response* response;
      response = net_http_get ("http", domain, "moefel.repo", 80, NULL);

      curl_global_cleanup();

      /* Only when the status code is 200 we should process the entry. */
      if (response->status != 200)
	{
	  puts ("Could not get repository information.");
	  dt_http_response_free (response);
	  return 1;
	}

      /* Parse the gathered information and store it in a 'dt_repository'. */
      dt_repository* repo;
      if (p_repository_from_buffer (&repo, response->body) == 0)
	{
	  printf ("Error while parsing packages from '%s'.\n", domain);
	  return 0;
	}

      /* Clean up the memory allocated for the HTTP response. */
      dt_http_response_free (response);

      /* Set the domain name. */
      repo->domain = calloc (1, strlen (domain) + 1);
      repo->domain = strcpy (repo->domain, domain);

      /* Set the current local time as 'created_at'. */
      repo->created_at = m_current_timestamp();

      /* Add the repository to the database. */
      if (!db_repositories_add (DATABASE_NAME, repo))
	printf ("Failed to add '%s'.\n", repo->name);

      /* Clean up the dynamically allocated memory. */
      dt_repository_free (repo);
    }

  /* OPTION: disable
   * ----------------------------------------------------------------------
   * This option gives the user the ability to remove a repository.
   */
  else if (!strcmp (argv[1], "disable"))
    {
      /* When the user hasn't provided enough arguments, show a 
       * usage message hinting on how to use this command. This macro
       * will prevent the rest of the code from executing. */
      if (argc <= 2 || !strcmp (argv[2], "--help"))
	show_usage ("get <package-name> (from <repository>)");

      db_repositories_disable (DATABASE_NAME, argv[2]);
    }


  /* OPTION: get
   * ----------------------------------------------------------------------
   * This option gives the user the ability to remove a repository.
   */
  else if (!strcmp (argv[1], "get"))
    {
      /* When the user hasn't provided enough arguments, show a 
       * usage message hinting on how to use this command. This macro
       * will prevent the rest of the code from executing. */
      if (argc <= 2 || !strcmp (argv[2], "--help"))
	show_usage ("get <domain.tld>");

      curl_global_init (CURL_GLOBAL_ALL);

      char* package = argv[2];
      char* repository = NULL;

      if (argc > 4 && !strcmp (argv[3], "from")) repository = argv[4];

      /* Get the package data from the local database. */
      dt_package* pkg;
      if (db_packages_get_by_name (DATABASE_NAME, package, repository, &pkg)
	  && pkg != NULL)
	{
	  puts ("Downloading package...");

	  char* pkg_full_name[5 + strlen (pkg->name)];
	  strcpy ((char *)&pkg_full_name, pkg->name);
	  strcat ((char *)&pkg_full_name, ".pkg");

	  /* Download the package. */
	  if (net_http_get_to_file ("http", pkg->domain, pkg->location, 80, (char *)&pkg_full_name))
	    {
	      puts ("Unpacking package...");

	      /* Extract it into the library. */
	      if (packagers_zip_unpack ((char *)&pkg_full_name, "PackageTest"))
		{
		  puts ("Done!");

		  /* Remove the compressed package from the filesystem. */
		  unlink ((char *)&pkg_full_name);
		}
	      else
		puts ("There was an error while unpacking the package.");
	    }
	  else
	    puts ("There was an error while downloading package.");
	}
      else
	puts ("There was an error while processing package data.");

      /* TODO: Add a size field to the packages table to be able to show a
       * progress bar. */

      dt_package_free (pkg);
      curl_global_cleanup();
    }
  

  /* OPTION: create
   * ----------------------------------------------------------------------
   * This option gives the user the ability to create a package.
   */
  else if (!strcmp (argv[1], "create"))
    {
      if (argc <= 3 || !strcmp (argv[2], "--help"))
	show_usage ("create <package-name.pkg> <directory>");

      const char* name = argv[2];
      const char* directory = argv[3];

      dt_package package;
      package.name = calloc (1, 255);
      package.description = calloc (1, 255);
      package.license = calloc (1, 255);
      package.category = calloc (1, 255);
      package.location = calloc (1, 255);
      package.checksum = NULL;

      /* Ask for package metadata. */
      printf ("Name: ");
      package.name = fgets (package.name, 255, stdin);

      printf ("Description: ");
      package.description = fgets (package.description, 255, stdin);
      
      printf ("License: ");
      package.license = fgets (package.license, 255, stdin);

      printf ("Category: ");
      package.category = fgets (package.category, 255, stdin);

      printf ("Location: ");
      package.location = fgets (package.location, 255, stdin);

      package.created_at = m_current_timestamp ();

      if (packagers_zip_pack (directory, name, &package))
	printf ("Package '%s' has been created.\n", name);
      else
	printf ("Package '%s' couldn't be created.\n", name);

      free (package.created_at);
      free (package.name), free (package.description), 
      free (package.license);
    }

  /* OPTION: list
   * ----------------------------------------------------------------------
   * This option gives the user the ability to list packages by repostory.
   */
  else if (!strcmp (argv[1], "list"))
    {
      dt_list* packages = NULL;
      if (argc > 2)
	db_packages_get_by_repository (DATABASE_NAME, argv[2], &packages);
      else
	db_packages_get_all (DATABASE_NAME, &packages);

      show_package_list (packages);
    }

  /* OPTION: search
   * ----------------------------------------------------------------------
   * This option gives the user the ability to list packages by repostory.
   */
  else if (!strcmp (argv[1], "search"))
    {
      if (argc <= 2 || !strcmp (argv[2], "--help"))
	show_usage ("search <keyword>");

      dt_list* packages = NULL;
      db_packages_get_by_keyword (DATABASE_NAME, argv[2], &packages);
      show_package_list (packages);
    }


  /* OPTION: extract
   * ----------------------------------------------------------------------
   * This option gives the user the ability to extract a package.
   */
  else if (!strcmp (argv[1], "extract"))
    {
      if (argc <= 3 || !strcmp (argv[2], "--help"))
	show_usage ("extract <package-name.pkg> <directory>");

      const char* name = argv[2];
      const char* directory = argv[3];

      if (packagers_zip_unpack (name, directory))
	printf ("Package '%s' has been unpacked.\n", name);
      else
	printf ("Package '%s' couldn't be unpacked.\n", name);
    }

  /* OPTION: version
   * ----------------------------------------------------------------------
   * This option shows versioning information.
   */
  else if (!strcmp (argv[1], "version"))
    show_version ();

  /* OPTION: *
   * ----------------------------------------------------------------------
   * When there's no matching command given, display the help message.
   */
  else
    {
      printf ("The program could not understand '%s'.\n", argv[1]);
      show_help ();
    }

  return 0;
}
