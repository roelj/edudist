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

#ifndef PARSERS_URI_H
#define PARSERS_URI_H

#include "../datatypes/http_response.h"

/**
 * @file   parsers/uri.h
 * @brief  A parser to extract parts of a URI.
 * @author Roel Janssen
 */

/**
 * This function extracts the parts in a URI.
 * When everything goes fine, it returns 1, otherwise it returns 0.
 */
int p_uri (const char* uri, char** host, char** location, char** protocol, int* port);

#endif//PARSERS_URI_H
