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

#ifndef DATABASE_REPOSITORIES_H
#define DATABASE_REPOSITORIES_H

/**
 * @file   database/repositories.h
 * @brief  An interface for adding repositories to a database.
 * @author Roel Janssen
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "../datatypes/repository.h"

/**
 * Using this function, you can add a repository to the local database.
 *
 * @param location   The location of the database file.
 * @param repository The data of the repository that should be added.
 * @return 'true' when everything went fine, 'false' when something went wrong.
 */
bool db_repositories_add (const char* location, dt_repository* repository);

/**
 * Using this function, you can disable a repository in the local database.
 *
 * @param location   The location of the database file.
 * @param domain     The domain name of the repository.
 * @return 'true' when everything went fine, 'false' when something went wrong.
 */
bool db_repositories_disable (const char* location, const char* domain);

/**
 * Using this function, you can get a 'dt_list' of all 'dt_repositories' in 
 * the database.
 *
 * @param location   The location of the database file.
 * @param repos      A place to store a 'dt_list' of the repositories.
 * @return 'true' when everything went fine, 'false' when something went wrong.
 */
bool db_repositories_get_all (const char* location, dt_list** repos);

/**
 * With this function, a list of repositories can be gathered by searching
 * for a keyword.
 *
 * @param location   The location of the database file.
 * @param keyword    The keyword to look for.
 * @param repos      A place to store a 'dt_list' of the repositories.
 * @return 'true' when everything went fine, 'false' when something went wrong.
 */
bool db_repositories_get_by_keyword (const char* location, const char* keyword,
				     dt_list** repos);


#ifdef __cplusplus
}
#endif

#endif//DATABASE_REPOSITORIES_H
