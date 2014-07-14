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

#ifndef DATATYPES_PACKAGE_H
#define DATATYPES_PACKAGE_H

#include <stddef.h>
#include "list.h"

/**
 * @file   datatypes/package.h
 * @brief  A struct for representing packages.
 * @author Roel Janssen
 */

/**
 * This data structure represents the data of a package.
 */
typedef struct
{
  /* Core members. */
  int id;
  int repository_id;
  int availability;
  char* name;
  char* description;
  char* license;
  char* location;
  char* checksum;
  char* created_at;

  /* Relationships. */
  dt_list* files;

} dt_package;

#endif//DATATYPES_PACKAGE_H
