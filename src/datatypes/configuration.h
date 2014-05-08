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

#ifndef DATATYPES_CONFIGURATION_H
#define DATATYPES_CONFIGURATION_H

#include "map.h"

/**
 * @file   datatypes/configuration.h
 * @brief  A struct for program-wide configuration variables.
 * @author Roel Janssen
 */

/**
 * This struct contains variables that should be accessible by all parts
 * of the program.
 */
typedef struct
{
  int num_repositories; /**< This variable contains the number of repositories
			     that are active. */
  int num_packages; /**< This variable contains the number of packages that are
		         active. */
  dt_map* repositories; /**< A pointer to a dt_map that contains the repositories. */
} dt_configuration;

#endif//DATATYPES_CONFIGURATION_H
