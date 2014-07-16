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

#include "uri.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*----------------------------------------------------------------------------.
 | P_URI                                                                      |
 '----------------------------------------------------------------------------*/
int
p_uri (const char* uri, char** host, char** location, char** protocol, int* port)
{
  int matches = sscanf (uri ,"%m[^:/]://%m[^:/]%ms", protocol, host, location);

  /* Only protocol and hostname matched. */
  if (matches == 2) *port = 80;

  /* Protocol, hostname and location matches. 
   * The location now possibly includes port information. extract it. */
  else if (matches == 3)
    {
      if (*location[0] == ':')
	{
	  char* loc = NULL;
	  if (sscanf (*location, ":%d%ms", port, &loc) == 2)
	    free (*location), *location = loc;
	}
      else
	*port = 80;
    }
  else
    return 0;

  /* Make sure 'location' is set. */
  if (*location == NULL)
    *location = calloc (1, 2), *location = memcpy (*location, "/", 1);

  return 1;
}
