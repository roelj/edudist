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

#include "strings.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <openssl/sha.h>

#define IS_BREAK_CHAR(c) (c == '\0' || c == '\n' || c == '\r')

void
m_strip_newline (char** input)
{
  if (*input == NULL) return;

  char* newline = strchr ((*input), '\r');
  if (newline == NULL) newline = strchr (*input, '\n');
  if (newline != NULL) (*input)[newline - *input] = '\0';
}

int
m_buffer_fgets (char** line, size_t length, const char* buffer, int* in)
{
  /* When either the buffer or the line buffer wasn't allocated,
   * this function would crash, prevent that. */
  if (buffer == NULL || *line == NULL) return 0;

  /* These variables are used to determine the length of the 
   * current string. */
  int start = *in;
  int total = 0;

  /* Find the next breaking character. */
  while (*in - start < length && !IS_BREAK_CHAR (buffer[*in])) *in += 1;

  /* Copy the string to the line buffer. */
  memset (*line, 0, length);
  memcpy (*line, buffer + start, *in - start);

  /* Step over the breaking character(s). */
  if (buffer[*in] != '\0')
    {
      *in += 1;
      if (IS_BREAK_CHAR (buffer[*in])) *in += 1, total -= 1;
    }

  /* Calculate the length without the breaking character. */
  total += *in - start - 1;
  return total;
}

int
m_buffer_sha256 (const char* buffer, size_t buffer_len, char checksum[65])
{
  /* Use an unsigned char to store the hash data in. */
  unsigned char hash[SHA256_DIGEST_LENGTH];

  /* Let OpenSSL do the heavy lifting here. */
  SHA256_CTX sha256;

  /* Initialize the SHA256_CTX structure. */
  if (SHA256_Init (&sha256) == 0) return 0;

  /* Pass the data to the SHA256_Update function to process it. */
  if (SHA256_Update (&sha256, buffer, buffer_len) == 0) return 0;

  /* Get the hash and clean up the SHA256_CTX structure. */
  if (SHA256_Final (hash, &sha256) == 0) return 0;

  /* Create a readable version of the hash and "convert" it to 
   * a signed char. */
  int i = 0;
  for(i = 0; i < SHA256_DIGEST_LENGTH; i++)
    sprintf (checksum + (i * 2), "%02x", hash[i]);

  /* Make sure the string is terminated properly. The hash is 64 bytes, and
   * we're setting the 65th byte to 0. */
  checksum[64] = 0;

  return 1;
}

int 
m_file_to_buffer (const char* filename, int* size, char** buffer)
{
  if (buffer == NULL) return 0;

  FILE* file = fopen (filename, "rb");
  if (file == NULL) return 0;

  fseek (file, 0L, SEEK_END);
  *size = ftell (file);

  fseek (file, 0L, SEEK_SET);
  *buffer = malloc (*size);
  if (*buffer != NULL)
    fread (*buffer, *size, 1, file);

  fclose (file);

  return 1;
}
