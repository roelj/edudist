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

#include "command.h"
#include "../parsers/uri.h"
#include "../network/http.h"
#include "../parsers/configuration.h"

#include <stdlib.h>
#include <string.h>

extern dt_configuration config;

int
h_command_add_repo (const char* uri)
{
  dt_http_response* response = calloc (1, sizeof (dt_http_response));
  if (!p_uri (uri, &response->host, &response->location, 
	     &response->protocol, &response->port))
    return 1;

  response = net_http_get (response->protocol, response->host,
			   response->location, response->port, response);

  p_configuration_from_data (&config, response->body, 
			     response->body_len, response->host);
  config.num_repositories++;

  dt_http_response_free (response);
  return 0;
}


int
h_command_get_from (const char* get, const char* from)
{
  char* url = calloc (1, strlen (get) + strlen (from) + 10);
  dt_http_response* response = calloc (1, sizeof (dt_http_response));

  /* Store the package to a file immediately. */
  net_http_get_to_file (response->protocol, response->host,
			response->location, response->port, get);

  free (url);
  return 1;
}
