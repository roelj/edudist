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

#ifndef PARSERS_PACKAGE_H
#define PARSERS_PACKAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../datatypes/package.h"
#include "../datatypes/list.h"
#include <stddef.h>

/**
 * @file   parsers/package.h
 * @brief  A set of parser functions to read and write package data.
 * @author Roel Janssen
 */

/**
 * This function reads a file and tries to parse package information from it.
 * There can only be one package per file. The file may not be compressed. To
 * read data from a package file use the 'packagers_unpack_*' function(s) to
 * unpack the package first.
 *
 * Unpacking is kept from here to avoid duplicating functionality.
 *
 * @param package An unallocated 'dt_package' to store package information.
 * @param path    The path to the file that should be parsed.
 *
 * @return 1 when everything went fine, 0 when something failed.
 */
int p_package_from_file (dt_package** package, const char* path);

/**
 * This function writes package information to a file so that it can be parsed
 * with 'p_package_from_file' later on.
 *
 * @param package The package data to write.
 * @param path    The path to the file that should be written to.
 * @return 1 when everything went fine, 0 when something failed.
 */
int p_package_to_file (dt_package* package, const char* path);

/**
 * This function writes package information to a string buffer. The 
 * 'p_package_to_file' uses this function too.
 *
 * @param package The package data to write.
 * @param output  A pointer to the buffer to write into.
 * @return 1 when everything went fine, 0 when something failed.
 */
int p_package_to_buffer (dt_package* package, char** output);

/**
 * This function reads package information from a buffer and puts the
 * packages in a 'dt_list'.
 *
 * @param packages The package data to write.
 * @param buffer   The buffer to read data from.
 * @return 1 when everything went fine, 0 when something failed.
 */
int p_packages_from_buffer (dt_list** packages, const char* input);

#ifdef __cplusplus
}
#endif

#endif//PARSERS_PACKAGE_H
