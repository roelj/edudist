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

#include "package.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LINE_LENGTH 255

static void
p_package_prepare (char** location, int amount)
{
  *location += amount;
  while (*location[0] == ' '
	 || *location[0] == ':'
	 || *location[0] == '\t'
	 || *location[0] == '=')
    *location += 1;

  char* newline = strchr ((*location), '\r');
  if (newline == NULL) newline = strchr (*location, '\n');
  if (newline != NULL) (*location)[newline - *location] = '\0';
}

int
p_package_from_file (dt_package** package, const char* path)
{
  /* Allocate memory for the package data. */
  *package = calloc (1, sizeof (dt_package));
  if (*package == NULL) return 0;

  /* This is a convenience variable for accessing the allocated package. */
  dt_package* pkg = *package;

  /* If we cannot access the file, return with error. */
  if (access (path, F_OK) == -1) return 0;

  /* Open the file specified by 'path'. */
  FILE* file;
  file = fopen (path, "r");
  if (file == NULL) return 0;

  /* Allocate memory for a line buffer. */
  char line[LINE_LENGTH];

  /* Read through the file line by line. */
  while (fgets ((char *)&line, LINE_LENGTH, file) != NULL)
    {
      /* Skip the line whenever it starts with a #. */
      if (line[0] == '#') continue;

      /* Figure out the useful data. */
      char* location;
      if ((location = strstr ((char *)&line, "Name")) != NULL)
	p_package_prepare (&location, 4),
	pkg->name = calloc (1, strlen (location) + 1),
	pkg->name = strncpy (pkg->name, location, strlen (location));

      else if ((location = strstr ((char *)&line, "Description")) != NULL)
	p_package_prepare (&location, 11),
	pkg->description = calloc (1, strlen (location) + 1),
	pkg->description = strncpy (pkg->description, location, strlen (location));

      else if ((location = strstr ((char *)&line, "License")) != NULL)
	p_package_prepare (&location, 7),
	pkg->license = calloc (1, strlen (location) + 1),
	pkg->license = strncpy (pkg->license, location, strlen (location));

      else if ((location = strstr ((char *)&line, "Location")) != NULL)
	p_package_prepare (&location, 8),
	pkg->location = calloc (1, strlen (location) + 1),
	pkg->location = strncpy (pkg->location, location, strlen (location));

      else if ((location = strstr ((char *)&line, "Timestamp")) != NULL)
	p_package_prepare (&location, 9),
	pkg->created_at = calloc (1, strlen (location) + 1),
	pkg->created_at = strncpy (pkg->created_at, location, strlen (location));
    }

  fclose (file);
  return 1;
}

int
p_package_to_file (dt_package* package, const char* path)
{
  FILE* file;
  file = fopen (path, "w");

  /* We we cannot write to the file, return with error. */
  if (file == NULL) return -1;

  /* Allocate enough memory to hold the entire contents of the file in 
   * memory. Writing to memory should be a lot faster than writing to disk,
   * so this should lead to a performance improvement later on. */
  size_t output_len = (strlen (package->name) + strlen (package->description) +
		       strlen (package->license) + strlen (package->location) +
		       strlen (package->created_at)) + 75;

  char* output = calloc (1, output_len + 1);

  /* If we cannot allocate enough memory, return with error. */
  if (output == NULL) return 0;

  /* Copy the data from the package to a single string.*/
  snprintf (output, output_len,
	    "Name        = %s\n"
	    "Description = %s\n"
	    "License     = %s\n"
	    "Location    = %s\n"
	    "Timestamp   = %s\n",
	    package->name, package->description, package->license, 
	    package->location, package->created_at);

  /* Write the string to the file. */
  fwrite (output, 1, strlen (output), file);

  /* Clean up. */
  fclose (file);
  free (output);
}
