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

#include "common.h"
#include <stddef.h>
#include <string.h>

void
p_prepare (char** location, int amount)
{
  if (*location == NULL) return;

  *location += amount;
  while (*location[0] == ' '
	 || *location[0] == ':'
	 || *location[0] == '\t'
	 || *location[0] == '=')
    *location += 1;

  char* newline = strchr ((*location), '\r');
  if (newline == NULL) newline = strchr (*location, '\n');
  if (newline != NULL) (*location)[newline - *location] = '\0';
}
