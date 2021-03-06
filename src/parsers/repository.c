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

#include "repository.h"
#include "common.h"
#include "../misc/strings.h"
#include "package.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#define LINE_LENGTH 255
#define FROM_BUFFER false
#define FROM_FILE   true
#define WIN32_TEMP_FILE "moefel-temp"

static int
p_repository_from (dt_repository** repository, const char* input, bool from)
{
  /* If we cannot access the file, return with error. */
  if (from == FROM_FILE && access (input, F_OK) == -1) return 0;

  /* Allocate memory to store the repository data. */
  *repository = calloc (1, sizeof (dt_repository));
  if (*repository == NULL) return 0;

  /* This is a convenience variable for accessing the allocated repository. */
  dt_repository* repo = *repository;
  char* data;

  /* When we're parsing a file, copy it to a buffer. */
  if (from == FROM_FILE)
    {
      FILE* file;
      file = fopen (input, "r");
      fseek (file, 0L, SEEK_END);
      size_t data_len = ftell (file);

      data = calloc (1, data_len + 1);
      if (data == NULL || fread (data, 1, data_len, file) != data_len) 
	return 0;

      fclose (file);
    }
  else
    data = (char *)input;

  /* Allocate memory for a line buffer. */
  char line[LINE_LENGTH];

  /* Read through the file line by line. */
  int count = 0;
  char* buffer = (char *)&line;
  while (m_buffer_fgets (&buffer, LINE_LENGTH, data, &count) > 0)
    {
      /* Skip the line whenever it starts with a #. */
      if (line[0] == '#') continue;

      /* Figure out the useful data. */
      char* location;
      if ((location = strstr ((char *)&line, "Repository")) != NULL)
	p_prepare (&location, 10),
	repo->name = calloc (1, strlen (location) + 1),
	repo->name = strncpy (repo->name, location, strlen (location));
    }

  /* Parse packages from the same input buffer. */
  p_packages_from_buffer (&repo->packages, data);

  /* Clean up the allocated memory. */
  if (from == FROM_FILE) free (data);

  return 1;  
}

int
p_repository_from_file (dt_repository** repository, const char* path)
{
  return p_repository_from (repository, path, FROM_FILE);
}

int
p_repository_from_buffer (dt_repository** repository, const char* buffer)
{
  return p_repository_from (repository, buffer, FROM_BUFFER);
}

int
p_repository_to_file (dt_repository* repository, const char* path)
{
  FILE* file;
  file = fopen (path, "w");

  /* We we cannot write to the file, return with error. */
  if (file == NULL) return 0;

  char* output;
  if (p_repository_to_buffer (repository, &output) == 0) return 0;

  /* Write the string to the file. */
  fwrite (output, 1, strlen (output), file);

  /* Clean up. */
  fclose (file);
  free (output);

  return 1;
}

int
p_repository_to_buffer (dt_repository* repository, char** output)
{
  /* This variable will contain the amount of memory to allocate. */
  size_t output_len;

  /* Identifiers and whitespacing. */
  output_len = 30;

  /* Remove whitespace and newline characters from the input. */
  p_prepare (&repository->name, 0);
  p_prepare (&repository->created_at, 0);

  /* Passing NULL to strlen() is forbidden, so make sure we don't do that. */
  if (repository->name != NULL) output_len += strlen (repository->name);
  if (repository->created_at != NULL) output_len += strlen (repository->created_at);

  /* Allocate enough memory to hold the entire contents of the file in 
   * memory. Writing to memory should be a lot faster than writing to disk,
   * so this should lead to a performance improvement later on. */
  *output = calloc (1, output_len + 1);

  /* If we cannot allocate enough memory, return with error. */
  if (*output == NULL) return 0;

  /* Copy the data of the repository to a single string. */
  snprintf (*output, output_len,
	    "Repository  = %s\n"
	    "Timestamp   = %s\n",
	    repository->name, repository->created_at);

  return 1;
}
