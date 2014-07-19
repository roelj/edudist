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

#ifndef MISC_STRINGS_H
#define MISC_STRINGS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

/**
 * @file   misc/strings.h
 * @brief  A set of functions that simplify string related stuff.
 * @author Roel Janssen
 */

/**
 * This function removes the newline character(s) from a given input,
 * setting them to '\0'. It will only remove the first occurence.
 *
 * @param input A pointer to the string that should be modified.
 */
void m_strip_newline (char** input);

/**
 * This function is effectively an fgets() like function for string buffers.
 * @param line   An allocated string to store the output line in.
 * @param length The maximum length of the output line.
 * @param buffer The buffer to look into.
 * @param in     An offset of where to start looking.
 */
int m_buffer_fgets (char** line, size_t length, const char* buffer, int* in);


#ifdef __cplusplus
}
#endif

#endif//MISC_STRINGS_H
