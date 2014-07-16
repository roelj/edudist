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

#ifndef MISC_DATETIME_H
#define MISC_DATETIME_H

#include "../datatypes/configuration.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file   misc/datetime.h
 * @brief  A set of functions that simplify time-related stuff.
 * @author Roel Janssen
 */

/**
 * This function returns a dynamically allocated string containing 
 * the current (local) timestamp in the format YYYY-MM-DD HH:II:SS.
 *
 * When something goes wrong this function will return NULL.
 */
char* m_current_timestamp ();

#ifdef __cplusplus
}
#endif

#endif//MISC_DATETIME_H
