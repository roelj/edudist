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

#include "library.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

int
fs_create_library_path (const char* db, char** path)
{
  if (!db_configuration_get (db, KEY_STORAGE_PATH, path)) return 0;

  #ifndef WIN32
  mode_t permissions = 0700; //S_IRWXU | S_IRWXG | S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
  if (mkdir (*path, permissions) == -1) return 0;
  #else
  if (mkdir (*path) == -1) return 0;
  #endif

  return 1;
}

int
fs_create_repository_path (const char* db, const char* repo, char** path)
{
  char* root = NULL;
  if (!db_configuration_get (db, KEY_STORAGE_PATH, &root)) return 0;

  /* Create the "full" path for the repository. */
  size_t repo_path_len = strlen (repo) + strlen (root) + 2;

  *path = calloc (1, repo_path_len);
  if (*path == NULL) return 0;

  strcpy (*path, root);
  if (root[strlen (root) - 1] == '/') strcat (*path, "/");
  strcat (*path, repo);

  #ifndef WIN32
  mode_t permissions = 0700; //S_IRWXU | S_IRWXG | S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
  if (mkdir (root, permissions) == -1 && mkdir (*path, permissions) == -1) 
    return 0;
  #else
  if (mkdir (root) == -1 && mkdir (*path) == -1) return 0;
  #endif

  return 1;
}
