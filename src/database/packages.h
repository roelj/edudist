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

#ifndef DATABASE_PACKAGES_H
#define DATABASE_PACKAGES_H

/**
 * @file   database/packages.h
 * @brief  An interface for adding packages to a database.
 * @author Roel Janssen
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "../datatypes/package.h"
#include "../datatypes/list.h"

/**
 * Using this function, you can add a package to the local database.
 *
 * @param location The location of the database file.
 * @param package  A dt_package.
 * @return 'true' when everything went fine, 'false' when something went wrong.
 */
bool db_packages_add (const char* location, dt_package* package);

/**
 * Using this function, you can add a list of packages to the local database.
 *
 * @param location The location of the database file.
 * @param packages A list of 'dt_package's.
 * @return 'true' when everything went fine, 'false' when something went wrong.
 */
bool db_packages_add_list (const char* location, dt_list* packages);

#ifdef __cplusplus
}
#endif

#endif//DATABASE_PACKAGES_H
