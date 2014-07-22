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
 * @param location      The location of the database file.
 * @param packages      A list of 'dt_package's.
 * @param repository_id The ID of the repository.
 * @return 'true' when everything went fine, 'false' when something went wrong.
 */
bool db_packages_add_list (const char* location, dt_list* packages, 
			   int repository_id);

/**
 * Using this function, you can easily create a custom result set.
 *
 * @param location      The location of the database file.
 * @param filter        Start with "WHERE" followed by your own narrowing.
 * @param packages      A pointer to a (empty) dt_list item.
 * @return 'true' when everything went fine, 'false' when something went wrong.
 */
bool db_packages_get_by_custom_filter (const char* location, const char* keyword,
				       dt_list** packages);

/**
 * Using this function, you can get a list of packages that belong to a 
 * particular repository.
 *
 * @param location      The location of the database file.
 * @param repository    The link of the repository.
 * @param packages      A pointer to a (empty) dt_list item.
 * @return 'true' when everything went fine, 'false' when something went wrong.
 */
bool db_packages_get_by_repository (const char* location, 
				    const char* repository, dt_list** packages);

/**
 * Using this function, you can get a list of packages that match with a 
 * particular keyword.
 *
 * @param location      The location of the database file.
 * @param keyword       The keyword to search for.
 * @param packages      A pointer to a (empty) dt_list item.
 * @return 'true' when everything went fine, 'false' when something went wrong.
 */
bool db_packages_get_by_keyword (const char* location, const char* keyword,
				 dt_list** packages);

/**
 * Using this function, you can get a list of packages all packages.
 *
 * @param location      The location of the database file.
 * @param packages      A pointer to a (empty) dt_list item.
 * @return 'true' when everything went fine, 'false' when something went wrong.
 */
bool db_packages_get_all (const char* location, dt_list** packages);

/**
 * Using this function, you can get a package by its name.
 *
 * @param location      The location of the database file.
 * @param name          The name of the package.
 * @param package       A pointer to a (empty) dt_package item.
 * @return 'true' when everything went fine, 'false' when something went wrong.
 */
bool db_packages_get_by_name (const char* location, const char* name, 
			      const char* repo, dt_package** package);

#ifdef __cplusplus
}
#endif

#endif//DATABASE_PACKAGES_H
