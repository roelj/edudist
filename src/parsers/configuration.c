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

#include "configuration.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*----------------------------------------------------------------------------.
 | P_CONFIGURATION_CUT_NEWLINE                                                |
 | This function is only available locally.                                   |
 '----------------------------------------------------------------------------*/
static inline void 
p_configuration_cut_newline (char* line)
{
  char* newline = strchr (line, '\r');
  if (newline == NULL) newline = strchr (line, '\n');
  if (newline != NULL) line[newline - line] = '\0';
}

/*----------------------------------------------------------------------------.
 | P_CONFIGURATION_FROM_FILE                                                  |
 '----------------------------------------------------------------------------*/
int
p_configuration_from_file (dt_configuration* config, const char* path)
{
  /* We we cannot access the file, return with error. */
  if (access (path, F_OK) == -1) return -1;

  FILE* file;
  file = fopen (path, "r");

  /* We we cannot access the file, return with error. */
  if (file == NULL) return -1;

  /* These variables are needed to parse the file line by line. */
  char* line = NULL;
  size_t line_len = 0;
  ssize_t read = 0;

  while ((read = getline (&line, &line_len, file)) != -1)
    {
      char* location = 0;
      if ((location = strstr (line, "Repository: ")) != NULL)
	{
	  p_configuration_cut_newline (line);
	  location += 12;
	  config->repositories = dt_map_add (config->repositories,
					     location,
					     strlen (location) + 1,
					     NULL);
	}
      else if ((location = strstr (line, "Package: ")) != NULL)
	{
	  p_configuration_cut_newline (line);
	  location += 9;
	  config->repositories->data = dt_map_add (config->repositories->data,
						   location,
						   strlen (location) + 1,
						   NULL);
	}

      /* The getline() function allocated memory, free it here. */
      free (line), line = NULL;
    }

  /* Free the last getline() read. */
  free (line), line = NULL;

  fclose (file);
  return 0;
}
