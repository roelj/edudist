/*
 * Copyright (C) 2013  Roel Janssen <roel@roelserve.com>
 *
 * This file is part of edudist.
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

#include "parsers/configuration.h"
#include "parsers/uri.h"
#include "network/http.h"
#include "high/command.h"
#include "database/installation.h"
#include "database/repositories.h"

/*----------------------------------------------------------------------------.
 | SHOW_HELP                                                                  |
 | This function displays the possible command-line arguments.                |
 '----------------------------------------------------------------------------*/
static void
show_help ()
{
  puts ("\nThe most commonly used commands are:\n"
	"    add       Add a repository.\n"
	"    create    Create a new package.\n"
	"    get       Retrieve a package.\n"
	"    version   Show versioning information.\n"
	"    help      Show this message.\n");
}

/*----------------------------------------------------------------------------.
 | SHOW_VERSION                                                               |
 | This function (implemented as a macro) displays the current version.       |
 '----------------------------------------------------------------------------*/
#define show_version() puts ("Version 0.1\n")

/*----------------------------------------------------------------------------.
 | SHOW_USAGE                                                                 |
 | This function (implemented as a macro) displays a usage message.           |
 | It will only work when called from within main().                          |
 '----------------------------------------------------------------------------*/
#define show_usage(msg) { printf ("Usage: %s %s\n", argv[0], msg); return 1; }

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

  /* This variable provides access to user-specific configuration data. */
  dt_configuration config;
  memset (&config, 0, sizeof (dt_configuration));

  /* Set up the database before we could possibly need its functionality. 
   * This function won't do anything when the file already exists. */
  db_setup ("edudist.db");

  /* OPTION: add
   * ----------------------------------------------------------------------
   * This option gives the user the ability to add a repository.
   */
  if (!strcmp (argv[1], "add"))
    {
      /* When the user hasn't provided enough arguments, show a 
       * usage message hinting on how to use this command. This macro
       * will prevent the rest of the code from executing. */
      if (argc <= 3 || !strcmp (argv[2], "--help"))
	show_usage ("add <name> <domain.tld>");

      /* These variables a purely there for a clearer understanding
       * of the code that follows. */
      const char* name = argv[2];
      const char* domain = argv[3];

      dt_repository repo;
      memset (&repo, 0, sizeof (dt_repository));

      repo.name = calloc (1, strlen (name) + 1);
      repo.name = strcpy (repo.name, name);

      repo.domain = calloc (1, strlen (domain) + 1);
      repo.domain = strcpy (repo.domain, domain);

      if (db_repositories_add ("edudist.db", &repo))
	printf ("'%s' has been added.\n", repo.name);
      else
	printf ("Failed to add '%s'.\n", repo.name);

      free (repo.name);
      free (repo.domain);
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

      if (packagers_zip_pack (directory, name))
	printf ("Package '%s' has been created.\n", name);
      else
	printf ("Package '%s' couldn't be created.\n", name);
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

  return 0;
}
