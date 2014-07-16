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

#ifndef DATABASE_INSTALLATION_H
#define DATABASE_INSTALLATION_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

/**
 * Setting up the database so it can be used by other database functions
 * in this program can be done using this function.
 *
 * @param location The location to store the database file.
 * @return 'true' when everything went fine, 'false' when something went wrong.
 */
bool db_setup (const char* location);

#ifdef __cplusplus
}
#endif

#endif//DATABASE_INSTALLATION_H
