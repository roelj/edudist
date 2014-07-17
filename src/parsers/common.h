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

#ifndef PARSERS_COMMON_H
#define PARSERS_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file   parsers/common.h
 * @brief  A set of parser-related functions that are used by other parsers.
 * @author Roel Janssen
 */

/**
 * This function 'prepares' a line of input by stripping whitespace and newline
 * characters. When 'location' is empty, this function won't do anything.
 *
 * @param location  A pointer to the input string.
 * @param amount    The amount of characters to skip.
 */
void p_prepare (char** location, int amount);

#ifdef __cplusplus
}
#endif

#endif//PARSERS_COMMON_H
