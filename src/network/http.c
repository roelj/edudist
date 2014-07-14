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

#include "http.h"
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define USER_AGENT "edudist"

/*----------------------------------------------------------------------------.
 | NET_HTTP_GET_HEADER                                                        |
 | This function is only available to other functions within NET_HTTP. It is  |
 | used to catch header data from a CURL action.                              |
 '----------------------------------------------------------------------------*/
static size_t
net_http_get_header (char *ptr, size_t size, size_t nmemb, void *stream)
{
  size_t data_len = size * nmemb;
  dt_http_response* data = *(dt_http_response**)stream;

  /* If data->header is NULL, the call is equivalent to a malloc. So we don't
   * need to check. */
  data->header = realloc (data->header, data->header_len + data_len + 1);
  assert (data->header != NULL);

  memcpy (data->header, ptr, data_len);
  data->header_len += data_len;
  data->header[data->header_len] = 0x0;

  return data_len;
}

/*----------------------------------------------------------------------------.
 | NET_HTTP_GET_DATA                                                          |
 | This function is only available to other functions within NET_HTTP. It is  |
 | used to catch data from a CURL action.                                     |
 '----------------------------------------------------------------------------*/
static size_t
net_http_get_data (char *ptr, size_t size, size_t nmemb, void *stream)
{
  size_t data_len = size * nmemb;

  dt_http_response* data = *(dt_http_response**)stream;

  data->body = realloc (data->body, data->body_len + data_len + 1);
  assert (data->body != NULL);

  memcpy (data->body, ptr, data_len);
  data->body_len += data_len;
  data->body[data->body_len] = '\0';

  return data_len;
}


/*----------------------------------------------------------------------------.
 | NET_HTTP_GET                                                               |
 | This function uses libcurl to do the heavy lifting.                        |
 '----------------------------------------------------------------------------*/
dt_http_response*
net_http_get (const char* protocol, const char* host, const char* location, 
	      int port, dt_http_response* response)
{
  if (response == NULL)
    response = calloc (1, sizeof (response));

  assert (response != NULL);

  /* Create a curl handle so we can use the curl functionality. */
  CURL* handle = curl_easy_init ();
  if (handle == NULL) return NULL;

  size_t url_len = strlen (host) + strlen (location) + strlen (protocol) + 10;
  char url[url_len];
  memset (url, 0, url_len);
  snprintf ((char *)&url, url_len, "%s://%s:%d/%s", protocol, host, port, location);

  curl_easy_setopt (handle, CURLOPT_URL, &url);

  /* Configure the behavior of curl to match what we need. */
  curl_easy_setopt (handle, CURLOPT_NOPROGRESS, 1);

  /* Store the content in memory. */
  curl_easy_setopt (handle, CURLOPT_WRITEFUNCTION, net_http_get_data);
  curl_easy_setopt (handle, CURLOPT_WRITEDATA, &response);

  /* Keep the header data seperately. */
  curl_easy_setopt (handle, CURLOPT_HEADERFUNCTION, net_http_get_header);
  curl_easy_setopt (handle, CURLOPT_WRITEHEADER, &response);

  /* Set a user-agent and follow redirects. */
  curl_easy_setopt (handle, CURLOPT_USERAGENT, USER_AGENT);
  curl_easy_setopt (handle, CURLOPT_FOLLOWLOCATION, 1);

  /* Perform the request. */
  curl_easy_perform (handle);

  /* Clean up the handle. The memory associated with it will be freed. */
  curl_easy_cleanup (handle);

  return response;
}

/*----------------------------------------------------------------------------.
 | NET_HTTP_GET_TO_FILE                                                       |
 | This function uses libcurl to do the heavy lifting.                        |
 '----------------------------------------------------------------------------*/
int net_http_get_to_file (const char* protocol, const char* host, 
			  const char* location, int port, const char* file)
{
  /* Create a curl handle so we can use the curl functionality. */
  CURL* handle = curl_easy_init ();
  if (handle == NULL) return 1;

  /* Open a file to write to. */
  FILE* filename = fopen (file, "wb");
  if (filename == NULL) return 1;

  size_t url_len = strlen (host) + strlen (location) + strlen (protocol) + 10;
  char url[url_len];
  memset (url, 0, url_len);

  snprintf ((char *)&url, url_len, "%s://%s:%d/%s", protocol, host, port, location);

  curl_easy_setopt (handle, CURLOPT_URL, &url);

  /* Configure the behavior of curl to match what we need. */
  curl_easy_setopt (handle, CURLOPT_NOPROGRESS, 1);

  /* Make sure the data is written to the file. */
  curl_easy_setopt (handle, CURLOPT_WRITEDATA, (void*)filename);

  /* Set a user-agent and follow redirects. */
  curl_easy_setopt (handle, CURLOPT_USERAGENT, USER_AGENT);
  curl_easy_setopt (handle, CURLOPT_FOLLOWLOCATION, 1);

  /* Perform the request. */
  curl_easy_perform (handle);

  /* Clean up the handle. The memory associated with it will be freed. */
  curl_easy_cleanup (handle);

  fclose (filename);

  return 0;
}
