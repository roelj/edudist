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

#include "http_response.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/*----------------------------------------------------------------------------.
 | DT_HTTP_RESPONSE_INIT                                                      |
 '----------------------------------------------------------------------------*/
dt_http_response*
dt_http_response_init (const char* protocol, const char* host, const char* location, int port)
{
  /* Allocate enough memory for the response. */
  dt_http_response* response = calloc (1, sizeof (dt_http_response));
  if (response == NULL) return NULL;

  /* Set the port. */
  response->port = port;

  /* Set the protocol. */
  if (protocol != NULL)
    {
      response->protocol = calloc (1, strlen (protocol) + 1);
      assert (response->protocol != NULL);
      response->protocol = memcpy (response->protocol, location, strlen (protocol));
    }
  else
    location = NULL;


  /* Set the host. */
  if (host != NULL)
    {
      response->host = calloc (1, strlen (host) + 1);
      assert (response->host != NULL);
      response->host = memcpy (response->host, host, strlen (host));
    }
  else
    host = NULL;

  /* Set the location. */
  if (location != NULL)
    {
      response->location = calloc (1, strlen (location) + 1);
      assert (response->location != NULL);
      response->location = memcpy (response->location, location, strlen (location));
    }
  else
    location = NULL;

  return response;
}

/*----------------------------------------------------------------------------.
 | DT_HTTP_RESPONSE_FREE                                                      |
 '----------------------------------------------------------------------------*/
void
dt_http_response_free (dt_http_response* response)
{
  /* When response is NULL, there's no point in going on. */
  if (response == NULL) return;
  
  if (response->host != NULL)
    free (response->host), 
      response->host = NULL;

  if (response->location != NULL)
    free (response->location), 
      response->location = NULL;

  if (response->header != NULL)
    free (response->header), 
      response->header = NULL, 
      response->header_len = 0;

  if (response->body != NULL)
    free (response->body), 
      response->body = NULL, 
      response->body_len = 0;

  if (response->protocol != NULL)
    free (response->protocol),
      response->protocol = 0;

  free (response);
}
