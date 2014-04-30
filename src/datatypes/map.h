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

#ifndef DATATYPES_MAP_H
#define DATATYPES_MAP_H

#include <stddef.h>

typedef struct
{
  void* key;
  void* next;
  void* data;
} dt_map;


/**
 * dt_map_add:
 * This function adds data to a dt_map*. It will automatically initialize
 * a map when you pass NULL for 'map'.
 */
dt_map* dt_map_add (dt_map* map, void* key, size_t key_len, void* data);

/**
 * dt_map_free:
 * This function frees all memory associated with a map.
 */
void dt_map_free (dt_map* map);

#endif//DATATYPES_MAP_H
