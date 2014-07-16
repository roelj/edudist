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

#ifndef DATATYPES_LIST_H
#define DATATYPES_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _dt_list
{
  void* data;
  struct _dt_list* next;
} dt_list;

dt_list* dt_list_append (dt_list* list, void* data);
dt_list* dt_list_prepend (dt_list* list, void* data);
dt_list* dt_list_remove (dt_list* list);
void dt_list_free (dt_list* list);
int dt_list_length (dt_list* list);
dt_list* dt_list_next (dt_list* list);

#ifdef __cplusplus
}
#endif

#endif//DATATYPES_LIST_H
