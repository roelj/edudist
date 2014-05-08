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
#include <getopt.h>
#include <curl/curl.h>

#include "parsers/configuration.h"
#include "parsers/uri.h"
#include "datatypes/configuration.h"
#include "datatypes/map.h"
#include "datatypes/http_response.h"
#include "network/http.h"
#include "high/command.h"

/* This global variable provides access to user-specific configuration data. */
dt_configuration config;

/*----------------------------------------------------------------------------.
 | SHOW_HELP                                                                  |
 | This function displays the possible command-line arguments.                |
 '----------------------------------------------------------------------------*/
static void
show_help ()
{
  printf ("\r\nAvailable options:\r\n"
	  "  --add-repo,        -a  Add a repository.\r\n"
	  "  --remove-repo,     -r  Remove a repository.\r\n"
	  "  --get,             -g  Get a package (see --from).\r\n"
	  "  --from,            -f  Set a filter for a single repository.\r\n"
	  "  --config,          -c  Read a configuration file.\r\n"
	  "  --version,         -v  Show versioning information.\r\n"
	  "  --help,            -h  Show this message.\r\n\r\n");
}

/*----------------------------------------------------------------------------.
 | SHOW_VERSION                                                               |
 | This function displays the program's current version.                      |
 '----------------------------------------------------------------------------*/
static void
show_version ()
{
  printf ("Version 0.1\r\n");
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

  /* These variables are used to keep track of which argument we're at. */
  int arg = 0;
  int index = 0;
  char* get_from = NULL;

  /*----------------------------------------------------------------------.
   | OPTIONS                                                              |
   | An array of structs that list all possible arguments that can be     |
   | provided by the user.                                                |
   '----------------------------------------------------------------------*/
  static struct option options[] =
    {
      { "add-repo",          required_argument, 0, 'a' },
      { "config",            required_argument, 0, 'c' },
      { "remove-repo",       required_argument, 0, 'r' },
      { "get",               required_argument, 0, 'g' },
      { "from",              required_argument, 0, 'f' },
      { "help",              no_argument,       0, 'h' },
      { "version",           no_argument,       0, 'v' },
      { 0,                   0,                 0, 0   }
    };

  while ( arg != -1 )
    {
      /* Make sure to list all short options in the string below. */
      arg = getopt_long (argc, argv, "a:c:r:hv", options, &index);

      switch (arg)
	{
	  /* OPTION: add-repo
	   *   This option provides a way to add a new repository to the local
	   *   distribution.
	   */
	case 'a':
	  if (optarg)
	    {
	      curl_global_init (CURL_GLOBAL_ALL);
	      h_command_add_repo (optarg);
	      curl_global_cleanup();
	    }
	  break;

	  /* OPTION: config
	   *   This option provides a way to use a different configuration file
	   *   than the default. 
	   */
	case 'c':
	  if (optarg)
	    p_configuration_from_file (&config, optarg);
	  break;

	  /* OPTION: remove-repo
	   *   This option provides a way to remove a repository from the local
	   *   distribution.
	   */
	case 'r':
	  if (optarg)
	    dt_map_remove (config.repositories, optarg, dt_map_free);
	  break;

	  /* OPTION: get
	   *   This option provides a way to get a package from a repository so
	   *   it can be used.
	   */
	case 'g':
	  if (optarg)
	    {
	      if (!get_from)
		get_from = optarg;
	      else
		h_command_get_from (optarg, get_from);
	    }
	  break;

	  /* OPTION: get
	   *   This option provides a way to get a package from a repository so
	   *   it can be used.
	   */
	case 'f':
	  if (optarg)
	    {
	      if (!get_from)
		get_from = optarg;
	      else
		h_command_get_from (get_from, optarg);
	    }
	  break;

	  /* OPTION: help
	   * This option displays the help message.
	   */
	case 'h':
	  show_help ();
	  break;

	  /* OPTION: version
	   * This option displays the program's version.
	   */
	case 'v':
	  show_version ();
	  break;
	}
    }

  p_configuration_to_file (&config, "test.conf");

  dt_map* repos = config.repositories;
  while (repos != NULL)
    {
      printf ("\r\nRepository: '%s'\r\n", (char*)repos->key);

      dt_map* pkgs = repos->data;
      while (pkgs != NULL)
	  printf ("  has package '%s'\r\n", (char*)pkgs->key),
	    pkgs = pkgs->next;

      repos = repos->next;
    }

  printf ("\r\n");
  printf ("Number of repositories: %d\r\n", config.num_repositories);
  printf ("Number of packages: %d\r\n", config.num_packages);

  dt_map_free (config.repositories, dt_map_free), config.repositories = NULL;

  return 0;
}
