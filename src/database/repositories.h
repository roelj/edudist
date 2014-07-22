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


#ifdef __cplusplus
}
#endif

#endif//DATABASE_REPOSITORIES_H
