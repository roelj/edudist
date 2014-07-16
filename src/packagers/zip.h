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

#ifndef PACKAGERS_ZIP_H
#define PACKAGERS_ZIP_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file   packagers/zip.h
 * @brief  A set of functions that simplify the packaging process.
 * @author Roel Janssen
 */

/**
 * This function simply unpacks a standard package from 'location' to 
 * 'destination'.
 *
 * @param location The location where the package is located.
 * @param destination The location where the package's content should be dropped.
 * @return 1 when everything went fine and 0 when something went wrong.
 */
int packagers_zip_unpack (const char* location, const char* destination);

/**
 * This function simply packs data in a directory specified by 'location'
 * and outputs a standard package to 'destination'.
 *
 * @param location The location the directory to pack.
 * @param destination The location where the package should be written to.
 * @return 1 when everything went fine and 0 when something went wrong.
 */
int packagers_zip_pack (const char* location, const char* destination);

#ifdef __cplusplus
}
#endif

#endif//PACKAGERS_ZIP_H
