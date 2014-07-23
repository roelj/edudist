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

/**
 * This function generates a readable SHA256 hash of data provided by buffer
 * for a length specified with buffer_len.
 *
 * @param buffer     The data to generate a SHA256 hash for.
 * @param buffer_len The length of the buffer.
 * @param checksum   A string of 65 characters that will contain the hash.
 *
 * @return 1 when everything went fine, 0 when something went wrong.
 */
int m_buffer_sha256 (const char* buffer, size_t buffer_len, char checksum[65]);

/**
 * This function copies the contents of a file to a string buffer.
 * You have to free() the buffer yourself.
 *
 * @param filename   The name of the file to open.
 * @param size       A pointer to an integer which will contain the size of
 *                   the file's content.
 * @param buffer     A pointer to a string buffer that will get allocated
 *                   and contain the contents of the file.
 *
 * @return 1 when everything went fine, 0 when something went wrong.
 */
int m_file_to_buffer (const char* filename, int* size, char** buffer);

#ifdef __cplusplus
}
#endif

#endif//MISC_STRINGS_H
