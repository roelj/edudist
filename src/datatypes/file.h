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

#ifndef DATATYPES_FILE_H
#define DATATYPES_FILE_H

#include <stddef.h>
//#include "list.h"

/**
 * @file   datatypes/file.h
 * @brief  A struct for representing files.
 * @author Roel Janssen
 */

/**
 * This data structure represents the data of a file.
 */
typedef struct
{
  /* Core members. */
  int id;
  int package_id;
  char* name;
  char* type;

  /* Relationships. */
  //dt_list* files;

} dt_file;

#endif//DATATYPES_FILE_H
