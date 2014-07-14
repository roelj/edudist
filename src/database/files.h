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

#ifndef DATABASE_FILES_H
#define DATABASE_FILES_H

/**
 * @file   database/files.h
 * @brief  An interface for adding files to a database.
 * @author Roel Janssen
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "../datatypes/file.h"
#include "../datatypes/list.h"

/**
 * Using this function, you can add a file to the local database.
 *
 * @param location The location of the database file.
 * @param file     A 'dt_file' to add.
 * @return 'true' when everything went fine, 'false' when something went wrong.
 */
bool db_files_add (const char* location, dt_file* file);

/**
 * Using this function, you can add a list of files to the local database.
 *
 * @param location The location of the database file.
 * @param files    A 'dt_list' of files.
 * @return 'true' when everything went fine, 'false' when something went wrong.
 */
bool db_files_add_list (const char* location, dt_list* files);

#ifdef __cplusplus
}
#endif

#endif//DATABASE_FILES_H
