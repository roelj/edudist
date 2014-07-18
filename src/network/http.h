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

#ifndef NETWORK_HTTP_H
#define NETWORK_HTTP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../datatypes/http_response.h"

/**
 * @file   network/http.h
 * @brief  Functions to do network communication using the HTTP protocol.
 * @author Roel Janssen
 */

/**
 * This function performs a HTTP GET request and returns the received data in a
 * 'dt_http_response'.
 *
 * @see dt_http_response
 *
 * @param protocol Either "http" or "https".
 * @param host     The hostname to send the request to.
 * @param location The location on the server that should be requested.
 * @param port     The port on which the web server is active.
 * @param response A response struct that should be used or NULL to create a 
 *                 new one.
 */
dt_http_response* net_http_get (const char* protocol, const char* host, 
				const char* location, int port, 
				dt_http_response* response);

/**
 * Instead of specifying every part of the url separately, this function
 * just processes a URL that is thrown at it. It's behavior is identical to
 * net_http_get().
 *
 * @param url      The URL to fetch.
 * @param response A response struct that should be used or NULL to create a
 *                 new one.
 */

dt_http_response* net_http_get_url (const char* url, 
				    dt_http_response* response);

/**
 * This function performs a HTTP GET request and writes the resulting data 
 * (without HTTP header) to 'file'.
 *
 * @param protocol Either "http" or "https".
 * @param host     The hostname to send the request to.
 * @param location The location on the server that should be requested.
 * @param port     The port on which the web server is active.
 * @param file     The file to store the data to.
 */
int net_http_get_to_file (const char* protocol, const char* host, 
			  const char* location, int port, const char* file);

#ifdef __cplusplus
}
#endif

#endif//NETWORK_HTTP_H
