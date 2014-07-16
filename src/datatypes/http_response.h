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

#ifndef DATATYPES_HTTP_RESPONSE_H
#define DATATYPES_HTTP_RESPONSE_H

#include <stddef.h>

/**
 * @file   datatypes/http_response.h
 * @brief  A struct to capture a HTTP response.
 * @author Roel Janssen
 */

/**
 * This structure can be used to store the data from a HTTP response.
 */
typedef struct
{
  /* Basic information about the request. */
  char* host; /**< The host used. */
  char* location; /**< The location used. */
  int port; /**< The port the web server was active at. */
  char* protocol; /**< The protocol used. */

  /* Variables to store the HTTP header. */
  char* header; /**< A variable to store the HTTP header in. */
  size_t header_len; /**< The length of the header variable. */

  /* Variables to store the HTTP body. */
  char* body; /**< A variable to store the HTTP body in. */
  size_t body_len; /**< The length of the body variable. */

} dt_http_response;

/**
 * This function makes it easier to initialize a 'dt_http_response'.
 *
 * @param[in] protocol The protocol for the connection.
 * @param[in] host     The hostname for the connection.
 * @param[in] location The location for the connection.
 * @param[in] port     The port for the connection.
 *
 * @return A @see dt_http_response struct containing the information provided.
 */
dt_http_response* dt_http_response_init (const char* protocol, const char* host,
					 const char* location, int port);

/**
 * This function makes it easier to clean up a 'dt_http_response'.
 * Don't forget to set 'response' to NULL after using this function.
 *
 * @param[in] response A @see dt_http_response to clean up.
 */
void dt_http_response_free (dt_http_response* response);


#endif//DATATYPES_HTTP_RESPONSE_H
