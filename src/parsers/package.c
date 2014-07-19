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

#include "package.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LINE_LENGTH 255

int
p_package_from_file (dt_package** package, const char* path)
{
  /* If we cannot access the file, return with error. */
  if (access (path, F_OK) == -1) return 0;

  /* Allocate memory for the package data. */
  *package = calloc (1, sizeof (dt_package));
  if (*package == NULL) return 0;

  /* This is a convenience variable for accessing the allocated package. */
  dt_package* pkg = *package;

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
      if ((location = strstr ((char *)&line, "Package")) != NULL)
	p_prepare (&location, 7),
	pkg->name = calloc (1, strlen (location) + 1),
	pkg->name = strncpy (pkg->name, location, strlen (location));

      else if ((location = strstr ((char *)&line, "Description")) != NULL)
	p_prepare (&location, 11),
	pkg->description = calloc (1, strlen (location) + 1),
	pkg->description = strncpy (pkg->description, location, strlen (location));

      else if ((location = strstr ((char *)&line, "License")) != NULL)
	p_prepare (&location, 7),
	pkg->license = calloc (1, strlen (location) + 1),
	pkg->license = strncpy (pkg->license, location, strlen (location));

      else if ((location = strstr ((char *)&line, "Category")) != NULL)
	p_prepare (&location, 8),
	pkg->category = calloc (1, strlen (location) + 1),
	pkg->category = strncpy (pkg->category, location, strlen (location));

      else if ((location = strstr ((char *)&line, "Homepage")) != NULL)
	p_prepare (&location, 8),
	pkg->homepage = calloc (1, strlen (location) + 1),
	pkg->homepage = strncpy (pkg->homepage, location, strlen (location));

      else if ((location = strstr ((char *)&line, "Timestamp")) != NULL)
	p_prepare (&location, 9),
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
  if (file == NULL) return 0;

  char* output;
  if (p_package_to_buffer (package, &output) == 0) return 0;

  /* Write the string to the file. */
  fwrite (output, 1, strlen (output), file);

  /* Clean up. */
  fclose (file);
  free (output);

  return 1;
}

int
p_package_to_buffer (dt_package* package, char** output)
{
  /* This variable will contain the amount of memory to allocate. */
  size_t output_len;

  /* Identifiers and whitespacing. */
  output_len = 90;

  /* Remove whitespace and newline characters from the input. */
  p_prepare (&package->name, 0);
  p_prepare (&package->description, 0);
  p_prepare (&package->license, 0);
  p_prepare (&package->category, 0);
  p_prepare (&package->homepage, 0);
  p_prepare (&package->created_at, 0);

  /* Passing NULL to strlen() is forbidden, so make sure we don't do that. */
  if (package->name != NULL) output_len += strlen (package->name);
  if (package->description != NULL) output_len += strlen (package->description);
  if (package->license != NULL) output_len += strlen (package->license);
  if (package->category != NULL) output_len += strlen (package->category);
  if (package->homepage != NULL) output_len += strlen (package->homepage);
  if (package->created_at != NULL) output_len += strlen (package->created_at);

  /* Allocate enough memory to hold the entire contents of the file in 
   * memory. Writing to memory should be a lot faster than writing to disk,
   * so this should lead to a performance improvement later on. */
  *output = calloc (1, output_len + 1);

  /* If we cannot allocate enough memory, return with error. */
  if (*output == NULL) return 0;

  /* Copy the data from the package to a single string.*/
  snprintf (*output, output_len,
	    "Package     = %s\n"
	    "Description = %s\n"
	    "License     = %s\n"
	    "Category    = %s\n"
	    "Homepage    = %s\n"
	    "Timestamp   = %s\n",
	    package->name, package->description, package->license, 
	    package->category, package->homepage, package->created_at);

  return 1;
}

int
p_packages_from_buffer (dt_list** packages, const char* input)
{
  /* Allocate memory for a line buffer. */
  char line[LINE_LENGTH];

  /* Allocate memory to store package information. */
  dt_package* pkg = calloc (1, sizeof (dt_package));
  if (pkg == NULL) return 0;

  /* Read through the file line by line. */
  int count = 0;
  char* buffer = (char *)&line;
  while (m_buffer_fgets (&buffer, LINE_LENGTH, input, &count) > 0)
    {
      /* Skip the line whenever it starts with a #. */
      if (line[0] == '#') continue;

      /* Figure out the useful data. */
      char* location;
      if ((location = strstr ((char *)&line, "Package")) != NULL)
	{
	  /* When there's a previous package, process it first. */
          if (pkg->name != NULL)
	    {
	      *packages = dt_list_prepend (*packages, pkg);
	      pkg = calloc (1, sizeof (dt_package));
	      if (pkg == NULL) break;
	    }

          p_prepare (&location, 7),
	  pkg->name = calloc (1, strlen (location) + 1),
	  pkg->name = strncpy (pkg->name, location, strlen (location));
        }
      else if ((location = strstr ((char *)&line, "Description")) != NULL)
	p_prepare (&location, 11),
	pkg->description = calloc (1, strlen (location) + 1),
	pkg->description = strncpy (pkg->description, location, strlen (location));

      else if ((location = strstr ((char *)&line, "License")) != NULL)
	p_prepare (&location, 7),
	pkg->license = calloc (1, strlen (location) + 1),
	pkg->license = strncpy (pkg->license, location, strlen (location));

      else if ((location = strstr ((char *)&line, "Category")) != NULL)
	p_prepare (&location, 8),
	pkg->category = calloc (1, strlen (location) + 1),
	pkg->category = strncpy (pkg->category, location, strlen (location));

      else if ((location = strstr ((char *)&line, "Homepage")) != NULL)
	p_prepare (&location, 8),
	pkg->homepage = calloc (1, strlen (location) + 1),
	pkg->homepage = strncpy (pkg->homepage, location, strlen (location));

      else if ((location = strstr ((char *)&line, "Timestamp")) != NULL)
	p_prepare (&location, 9),
	pkg->created_at = calloc (1, strlen (location) + 1),
	pkg->created_at = strncpy (pkg->created_at, location, strlen (location));
    }
  
  /* Add the last package to the list. */
  if (pkg != NULL) *packages = dt_list_prepend (*packages, pkg);

  return 1;
}
