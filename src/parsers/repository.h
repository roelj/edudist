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

#ifndef PARSERS_REPOSITORY_H
#define PARSERS_REPOSITORY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../datatypes/repository.h"
#include <stddef.h>

/**
 * @file   parsers/repository.h
 * @brief  A set of parser functions to read and write repository data.
 * @author Roel Janssen
 */

/**
 * This function reads a file and tries to parse repository information from it.
 * There can only be one repository per file. 
 *
 * @param repository An unallocated 'dt_repository' to store repository information.
 * @param path       The path to the file that should be parsed.
 *
 * @return 1 when everything went fine, 0 when something failed.
 */
int p_repository_from_file (dt_repository** repository, const char* path);

/**
 * This function reads a buffer and tries to parse repository information from it.
 * There can only be one repository in the buffer. 
 *
 * @param repository An unallocated 'dt_repository' to store repository information.
 * @param buffer     The buffer where repository is available.
 *
 * @return 1 when everything went fine, 0 when something failed.
 */
int p_repository_from_buffer (dt_repository** repository, const char* buffer);

/**
 * This function writes repository information to a file so that it can be parsed
 * with 'p_repository_from_file' later on.
 *
 * @param repository The repository data to write.
 * @param path       The path to the file that should be written to.
 * @return 1 when everything went fine, 0 when something failed.
 */
int p_repository_to_file (dt_repository* repository, const char* path);

/**
 * This function writes repository information to a string buffer. The 
 * 'p_repository_to_file' uses this function too.
 *
 * @param repository The repository data to write.
 * @param output  A pointer to the buffer to write into.
 * @return 1 when everything went fine, 0 when something failed.
 */
int p_repository_to_buffer (dt_repository* repository, char** output);

#ifdef __cplusplus
}
#endif

#endif//PARSERS_REPOSITORY_H
