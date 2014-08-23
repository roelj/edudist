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

#ifndef HIGH_COMMAND_H
#define HIGH_COMMAND_H

#include "../parsers/repository.h"
#include "../network/http.h"
#include "../database/repositories.h"
#include "../misc/datetime.h"
#include "../datatypes/package.h"

#include <curl/curl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @file   high/command.h
 * @brief  Functions that resemble the command-line interface. This makes it
 *         easy to performs tasks internally or create a wrapper to another
 *         language.
 * @author Roel Janssen
 */

enum {
  E_NETWORK = -4,
  E_PARSER,
  E_DATABASE,
  E_UNKNOWN
} H_COMMAND_ERROR;

/**
 * This function can be used to add or enable a repository.
 * @param domain The domain of the repository you want to add or enable.
 * @return 1 when everything went fine, H_COMMAND_ERROR when something went
 *         wrong.
 */
int h_command_enable (const char* domain);

/**
 * This function can be used to update repositories.
 * @return 1 when everything went fine, H_COMMAND_ERROR when something went
 *         wrong.
 */
int h_command_update ();

/**
 * This function gets a package from a remote repository and unpacks it
 * in the user's library.
 *
 * @param package  The package name to get.
 * @param repo     The repository to get it from.
 *
 * @return 1 when everything went fine, H_COMMAND_ERROR when something went
 *         wrong.
 */
int h_command_get (const char* package, const char* repo);

#endif//HIGH_COMMAND_H

