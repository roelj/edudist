/*
 * Copyright (C) 2014  Roel Janssen <roel@moefel.org>
 *
 * This file is part of moefel
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

#include "map.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>


/*----------------------------------------------------------------------------.
 | DT_MAP_ADD                                                                 |
 | This function leans heavily upon having enough memory available. It uses   |
 | assert() to test whether we have enough memory available. These assertions |
 | are not included in the executable when it's built without debugging       |
 | information.                                                               |
 '----------------------------------------------------------------------------*/
dt_map*
dt_map_add (dt_map* map, void* key, size_t key_len, void* data)
{
  dt_map* map_item = NULL;

  /* Allocate memory for the new map item. */
  map_item = calloc (1, sizeof (dt_map));

  /* The 'assert' function is used on various strategic points in this code to
   * debug memory allocation problems. When this program runs out of memory,
   * there's no proper way of recovering anyway. */
  assert (map_item != NULL);

  /* Copy the key. */
  map_item->key = calloc (1, key_len);
  assert (map_item->key != NULL);
  map_item->key = memcpy (map_item->key, key, key_len);

  /* Reference the data. */
  map_item->data = data;

  /* Reference the next item. */
  map_item->next = map;

  return map_item;
}


/*----------------------------------------------------------------------------.
 | DT_MAP_REMOVE                                                              |
 '----------------------------------------------------------------------------*/
dt_map*
dt_map_remove (dt_map* map, void* key, void* callback)
{
  dt_map* tmp = map;
  while (tmp != NULL)
    {
      if (!strcmp ((char *)tmp->key, (char *)key))
	{
	  /* Free the memory for the key. */
	  free (tmp->key), tmp->key = NULL;

	  /* Handle the callback function. */
	  if (callback)
	    {
	      if (callback == dt_map_free)
		dt_map_free (tmp->data, NULL);
	      else
		{
		  void (*return_call) (void*) = callback;
		  return_call (tmp->data);
		}
	    }

	  /* Point the element to the next and free it. */
	  dt_map* item = tmp;
	  tmp = tmp->next;
	  free (item);

	  /* Stop looking for another key. */
	  break;
	}
    }

  return map;
}


/*----------------------------------------------------------------------------.
 | DT_MAP_FREE                                                                |
 | Make sure to set 'map' to NULL after using this function.                  |
 '----------------------------------------------------------------------------*/
void
dt_map_free (dt_map* map, void* callback)
{
  while (map != NULL)
    {
      free (map->key);

      if (callback)
	{
	  /* 'dt_map_free' is allowed too. */
	  if (callback == dt_map_free)
	    dt_map_free (map->data, NULL);
	  else
	    {
	      void (*return_call) (void*) = callback;
	      return_call (map->data);
	    }
	}

      dt_map* item = map;
      map = map->next;
      free (item);
    }

  free (map);
}
