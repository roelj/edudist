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
