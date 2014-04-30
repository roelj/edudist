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

#ifndef PARSERS_CONFIGURATION_H
#define PARSERS_CONFIGURATION_H

#include "../datatypes/configuration.h"
/** 
 * This function reads a file and tries to parse configuration items from it.
 * Returns 0 when everything went fine, -1 when something fails.
 */
int p_configuration_from_file (dt_configuration* config, const char* path);

#endif//PARSERS_CONFIGURATION_H
