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

#ifndef PARSERS_CONFIGURATION_H
#define PARSERS_CONFIGURATION_H

#include "../datatypes/configuration.h"
#include <stddef.h>

/**
 * @file   parsers/configuration.h
 * @brief  A set of parser functions to read and write configuration data.
 * @author Roel Janssen
 */

/**
 * This function reads a file and tries to parse configuration items from it.
 * Returns 0 when everything went fine, -1 when something failed.
 */
int p_configuration_from_file (dt_configuration* config, const char* path);

/**
 * This function tries to parse configuration from a string. When 
 * 'repository' is not NULL, any "Repository" fields will be ignored and
 * all packages will be added to the provided repository. This is mainly
 * a safety measurement to prevent cross-repository infection.
 * Returns 0 when everything went fine, -1 when something failed.
 */
int p_configuration_from_data (dt_configuration* config, const char* data, 
			       size_t data_len, const char* repository);

/**
 * This function writes a configuration to a file so that it can be parsed 
 * with 'p_configuration_from_file' later on.
 *
 * Returns 0 when everything went fine, -1 when something failed.
 */
int p_configuration_to_file (dt_configuration* config, const char* path);

#endif//PARSERS_CONFIGURATION_H
