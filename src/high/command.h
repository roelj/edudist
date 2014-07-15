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

#ifndef HIGH_COMMAND_H
#define HIGH_COMMAND_H

#include "../datatypes/configuration.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file   high/command.h
 * @brief  A set of functions that simplify the handling of commands.
 * @author Roel Janssen
 */

/**
 * This function is a wrapper for handling the 'add-repo' command-line option.
 * Returns 0 when everything went fine, 1 when something went wrong.
 */
int h_command_add_repo (dt_configuration *config, const char* uri);


/**
 * This function is a wrapper for handling the 'remove-repo' command-line 
 * option. Returns 1 when everything went fine, 0 when something went wrong.
 */
int h_command_remove_repo (const char* name);

/**
 * This function is a wrapper for handling the 'get' and 'from' command-line 
 * options. Returns 1 when everything went fine, 0 when something went wrong.
 */
int h_command_get_from (const char* get, const char* from);

#ifdef __cplusplus
}
#endif

#endif//HIGH_COMMAND_H
