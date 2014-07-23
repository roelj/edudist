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

#ifndef FILESYSTEM_LIBRARY_H
#define FILESYSTEM_LIBRARY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include "../database/repositories.h"
#include "../database/configuration.h"

/**
 * @file   filesystem/library.h
 * @brief  A set of functions that simplify constructing and maintaining 
 *         files of "the library" on the filesystem.
 * @author Roel Janssen
 */

/**
 * This function creates the directory for the library's root according to
 * the settings found in the database. When the directory already exists,
 * this function does nothing except for returning as if everything went fine.
 *
 * @param db The location to the database file containing configuration values.
 * @param path A string pointer that will contain the path that was created.
 * @return 1 when the directory exists (or has been created), 0 when the 
 *         directory doesn't exists  (or couldn't be created).
 */
int fs_create_library_path (const char* db, char** path);

/**
 * This function creates the directory for the given repository according to
 * the settings found in the database. When the directory already exists,
 * this function does nothing except for returning as if everything went fine.
 *
 * @param db   The location to the database file containing config values.
 * @param repo The repository's domain.
 * @param path A string pointer that will contain the path that was created.
 * @return 1 when the directory exists (or has been created), 0 when the 
 *         directory doesn't exists  (or couldn't be created).
 */
int fs_create_repository_path (const char* db, const char* repo, char** path);

#ifdef __cplusplus
}
#endif

#endif//FILESYSTEM_LIBRARY_H
