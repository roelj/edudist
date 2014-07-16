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

#ifndef DATATYPES_MAP_H
#define DATATYPES_MAP_H

#include <stddef.h>

/**
 * @file   datatypes/map.h
 * @brief  A singly-list with a data identifier implementation.
 * @author Roel Janssen
 */

/**
 * This struct is the base for a type that can store data identified by a key
 * and chain it in a singly-linked list. So it is basically a singly-linked 
 * list plus a key for each list item.
 */
typedef struct
{
  void* key;  /**< An identifier for the data. */
  void* next; /**< A pointer to the next element in the map. */
  void* data; /**< A pointer to the data of this map item. */
} dt_map;


/**
 * This function adds data to a dt_map. It will automatically initialize
 * a map when you pass NULL for 'map'.
 *
 * @param map     The dt_map to add the element to.
 * @param key     The key to set for this element.
 * @param key_len The length of the key.
 * @param data    The data for this element.
 * 
 * @return A dt_map element with the new item.
 */
dt_map* dt_map_add (dt_map* map, void* key, size_t key_len, void* data);


/**
 * This function removes an element from a map identified by 'key'.
 * 
 * It will only remove the first element with a matching key. If your 
 * 'data' part was dynamically allocated, you can pass a callback function
 * to free it. The callback function takes only one argument of type void*.
 *
 * @param map      The dt_map to remove the element from.
 * @param key      The key to search for.
 * @param callback An callback function to call to clean up the item's data.
 * 
 * @return The dt_map without the removed element.
 */
dt_map* dt_map_remove (dt_map* map, void* key, void* callback);


/**
 * This function frees all memory associated with a map. If your 'data'
 * part was dynamically allocated, you can pass a callback function to
 * free it. You can also pass 'dt_map_free' to it, it will use NULL as
 * the second argument.
 *
 * @param map      The dt_map to remove the element from.
 * @param callback An callback function to call to clean up the item's data.
 */
void dt_map_free (dt_map* map, void* callback);


#endif//DATATYPES_MAP_H
