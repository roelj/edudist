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
#include <ctype.h>
#include <string.h>
#include <unistd.h>

/*----------------------------------------------------------------------------.
 | P_CONFIGURATION_CUT_NEWLINE                                                |
 | This function is only available locally.                                   |
 '----------------------------------------------------------------------------*/
static void 
p_configuration_cut_newline (char* line)
{
  char* newline = strchr (line, '\r');
  if (newline == NULL) newline = strchr (line, '\n');
  else line[newline - line] = '\0';
}

/*----------------------------------------------------------------------------.
 | P_CONFIGURATION_FROM_FILE                                                  |
 '----------------------------------------------------------------------------*/
int
p_configuration_from_file (dt_configuration* config, const char* path)
{
  /* If we cannot access the file, return with error. */
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
      /* Skip the line whenever it starts with a #. */
      if (line[0] == '#') continue;

      char* location = 0;
      if ((location = strstr (line, "Repository: ")) != NULL)
	{
	  p_configuration_cut_newline (line);
	  location += 12;
	  config->repositories = dt_map_add (config->repositories,
					     location,
					     strlen (location) + 1,
					     NULL);
	  config->num_repositories++;
	}
      else if ((location = strstr (line, "Package: ")) != NULL)
	{
	  p_configuration_cut_newline (line);
	  location += 9;
	  config->repositories->data = dt_map_add (config->repositories->data,
						   location,
						   strlen (location) + 1,
						   NULL);
	  config->num_packages++;
	}

      /* The getline() function allocated memory, free it here. */
      free (line), line = NULL;
    }

  /* Free the last getline() read. */
  free (line), line = NULL;

  fclose (file);
  return 0;
}

/*----------------------------------------------------------------------------.
 | P_CONFIGURATION_FROM_DATA                                                  |
 '----------------------------------------------------------------------------*/
int
p_configuration_from_data (dt_configuration* config, const char* data, 
			   size_t data_len, const char* repository)
{
  /* When the data is empty, return with error. */
  if (data == NULL) return -1;

  /* Add the provided repository, if any. */
  if (repository != NULL)
    config->repositories = dt_map_add (config->repositories,
				       (char *)repository,
				       strlen (repository) + 1,
				       NULL);

  /* These variables are needed to parse the file line by line. */
  char* line = NULL;
  size_t read = 0;

  //while ((read = getline (&line, &line_len, file)) != -1)
  while (read <= data_len)
    {
      line = calloc (1, 255);

      int a = 0;
      for (; a < 255 && read <= data_len; a++)
	{
	  if (!isprint (data[read]))
	    {
	      read++, line[a] = '\0';
	      break;
	    }
	  else
	    line[a] = data[read];
	  read++;
	}

      /* Skip the line whenever it starts with a #. */
      if (line[0] == '#') continue;      

      char* location = 0;
      if (!repository && (location = strstr (line, "Repository: ")))
	{
	  p_configuration_cut_newline (line);
	  location += 12;
	  config->repositories = dt_map_add (config->repositories,
					     location,
					     strlen (location) + 1,
					     NULL);
	  config->num_repositories++;
	}

      else if ((location = strstr (line, "Package: ")) != NULL)
	{
	  p_configuration_cut_newline (line);
	  location += 9;
	  config->repositories->data = dt_map_add (config->repositories->data,
						   location,
						   strlen (location) + 1,
						   NULL);
	  config->num_packages++;
	}

      /* The getline() function allocated memory, free it here. */
      free (line), line = NULL;
    }

  /* Free the last getline() read. */
  free (line), line = NULL;
  return 0;
}


/*----------------------------------------------------------------------------.
 | P_CONFIGURATION_TO_FILE                                                    |
 '----------------------------------------------------------------------------*/
int
p_configuration_to_file (dt_configuration* config, const char* path)
{
  FILE* file;
  file = fopen (path, "w");

  /* We we cannot write to the file, return with error. */
  if (file == NULL) return -1;

  /* Allocate enough memory to store the entire output in. Writing chunks to 
   * memory should be a lot faster than writing to disk. */
  size_t output_len = (config->num_repositories + config->num_packages) * 50 + 1;
  char* output = calloc (1, output_len);

  /* If we cannot allocate enough memory, return with error. */
  if (output == NULL) return -1;

  /* This variables keeps track of how much data was written. */
  int written = 0;

  dt_map* repos = config->repositories;
  while (repos != NULL)
    {
      written += sprintf (output + written, "Repository: %s\r\n", (char*)repos->key);

      dt_map* pkgs = repos->data;
      while (pkgs != NULL)
	{
	  written += sprintf (output + written, "  > Package: %s\r\n", (char*)pkgs->key);
	  pkgs = pkgs->next;
	  if (pkgs == NULL) written += sprintf (output + written, "\r\n");
	}

      repos = repos->next;
    }

  fwrite (output, strlen (output), 1, file);
  fclose (file);
  free (output), output = NULL;

  return 0;
}
