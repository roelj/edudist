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

/* This global variable provides access to user-specific configuration data. */
dt_configuration config;

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
 | This function displays the program's current version.                      |
 '----------------------------------------------------------------------------*/
static void
show_version ()
{
  puts ("Version 0.1\n");
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
  if (argc < 2)
    {
      show_help ();
      return 0;
    }

  /* Set up the database before we could possibly need its functionality. 
   * This function won't do anything when the file already exists. */
  db_setup ("edudist.db");

  /*----------------------------------------------------------------------.
   | OPTIONS                                                              |
   | The command-line options are supposed to be used interactively.      |
   | Therefore I chose not to use the getopt functionality that most GNU  |
   | programs seem to follow.                                             |
   |                                                                      |
   | I find Git to be a pretty good example of how it should work.        |
   '----------------------------------------------------------------------*/

  /* OPTION: add
   *   This option gives the user the ability to add a repository.
   */
  if (!strcmp (argv[1], "add"))
    {
      if (argc > 2)
	{
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
    }

  /* OPTION: create
   *   This option gives the user the ability to create a package.
   */
  else if (!strcmp (argv[1], "create"))
    {
      if (argc > 3)
	{
	  const char* name = argv[2];
	  const char* directory = argv[3];

	  if (packagers_zip_pack (directory, name))
	    printf ("Package '%s' has been created.\n", name);
	  else
	    printf ("Package '%s' couldn't be created.\n", name);
	}
    }

  else if (!strcmp (argv[1], "extract"))
    {
      if (argc > 3)
	{
	  const char* name = argv[2];
	  const char* directory = argv[3];

	  if (packagers_zip_unpack (name, directory))
	    printf ("Package '%s' has been unpacked.\n", name);
	  else
	    printf ("Package '%s' couldn't be unpacked.\n", name);
	}
    }

  else if (!strcmp (argv[1], "help"))
    show_help ();

  else if (!strcmp (argv[1], "version"))
    show_version ();

  p_configuration_to_file (&config, "test.conf");

  dt_map* repos = config.repositories;
  while (repos != NULL)
    {
      printf ("\nRepository: '%s'\n", (char*)repos->key);

      dt_map* pkgs = repos->data;
      while (pkgs != NULL)
	  printf ("  has package '%s'\n", (char*)pkgs->key),
	    pkgs = pkgs->next;

      repos = repos->next;
    }

  printf ("Number of repositories: %d\n", config.num_repositories);
  printf ("Number of packages: %d\n", config.num_packages);

  dt_map_free (config.repositories, dt_map_free), config.repositories = NULL;

  return 0;
}
