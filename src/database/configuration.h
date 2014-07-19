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

#ifndef DATABASE_CONFIGURATION_H
#define DATABASE_CONFIGURATION_H

/**
 * @file   database/configuration.h
 * @brief  An interface for interacting with configuration values in database.
 * @author Roel Janssen
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

/**
 * For efficiency's sake, the keys to configuration items are stored as
 * integers in the database. Their meanings are defined with these macros.
 */
#define KEY_STORAGE_PATH 1

/**
 * Using this function, you can set the configuration item's values to 
 * their default values. Please note that some settings will be guessed
 * by accessing environment variables.
 *
 * @param location   The location of the database file.
 * @return 'true' when everything went fine, 'false' when something went wrong.
 */
bool db_configuration_set_defaults (const char* location);

/**
 * Using this function, you can change a configuration item's value.
 *
 * @param location   The location of the database file.
 * @param key        The configuration item's key.
 * @param value      The value to set.
 * @return 'true' when everything went fine, 'false' when something went wrong.
 */
bool db_configuration_set (const char* location, int key, const char* value);

/**
 * Using this function, you can get a configuration item's value.
 *
 * @param location   The location of the database file.
 * @param key        The configuration item's key.
 * @param value      A pointer to an unallocated string to store the value in.
 * @return 'true' when everything went fine, 'false' when something went wrong.
 */
bool db_configuration_get (const char* location, int key, char** value);

#ifdef __cplusplus
}
#endif

#endif//DATABASE_CONFIGURATION_H
