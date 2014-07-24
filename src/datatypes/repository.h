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

#ifndef DATATYPES_REPOSITORY_H
#define DATATYPES_REPOSITORY_H

/**
 * @file   datatypes/repository.h
 * @brief  A struct for representing repositories;
 * @author Roel Janssen
 */

#include "list.h"

/**
 * This data structure represents the data of a repository.
 */
typedef struct
{
  /* Core members. */
  int id;
  char* name;
  char* domain;
  char* created_at;
  int is_enabled;

  /* Relationships. */
  dt_list* packages;

} dt_repository;

/**
 * This function properly cleans up an instance of a 'dt_repository'.
 * @param repo  The instance to be cleaned up.
 */
void dt_repository_free (dt_repository* repo);

#endif//DATATYPES_REPOSITORY_H
