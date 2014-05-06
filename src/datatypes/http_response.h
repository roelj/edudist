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

#ifndef DATATYPES_HTTP_RESPONSE_H
#define DATATYPES_HTTP_RESPONSE_H

#include <stddef.h>

/**
 * This structure can be used to store the data from a HTTP response.
 */
typedef struct
{
  /* Basic information about the request. */
  char* host;
  char* location;
  int port;
  int status;
  char* protocol;

  /* Variables to store the HTTP header. */
  char* header;
  size_t header_len;

  /* Variables to store the HTTP body. */
  char* body;
  size_t body_len;

} dt_http_response;

/**
 * This function makes it easier to initialize a 'dt_http_response'.
 */
dt_http_response* dt_http_response_init (const char* protocol, const char* host,
					 const char* location, int port);

/**
 * This function makes it easier to clean up a 'dt_http_response'.
 * Don't forget to set 'response' to NULL after using this function.
 */
void dt_http_response_free (dt_http_response* response);


#endif//DATATYPES_HTTP_RESPONSE_H
