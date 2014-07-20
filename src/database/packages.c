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

#include "packages.h"
#include "files.h"

#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool
db_packages_add (const char* location, dt_package* package)
{
  sqlite3* db;
  sqlite3_stmt* result;
  int status;

  status = sqlite3_open (location, &db);
  if (status != SQLITE_OK) return false;

  /* By using a prepared statement we avoid SQL injection and double memory
   * allocation of the query string. */
  status = sqlite3_prepare_v2 (db, 
    "INSERT INTO packages (repository_id, availability, name, description, "
    "license, category, homepage, checksum) VALUES (?, ?, ?, ?, ?, ?, ?, ?)",
    -1, &result, NULL);

  if (status != SQLITE_OK) return false;

  /* Bind the values to the parameters. Apparantly the first index is 1. */
  sqlite3_bind_int (result, 1, package->repository_id);
  sqlite3_bind_int (result, 2, package->availability);
  sqlite3_bind_text (result, 3, package->name, -1, NULL);
  sqlite3_bind_text (result, 4, package->description, -1, NULL);
  sqlite3_bind_text (result, 5, package->license, -1, NULL);
  sqlite3_bind_text (result, 6, package->category, -1, NULL);
  sqlite3_bind_text (result, 7, package->homepage, -1, NULL);
  sqlite3_bind_text (result, 8, package->checksum, -1, NULL);

  /* Execute the query. */
  status = sqlite3_step (result);
  if (status != SQLITE_DONE) return false;

  sqlite3_finalize (result); 
  sqlite3_close(db);

  if (package->files != NULL)
    db_files_add_list (location, package->files);

  return true;
}

bool
db_packages_add_list (const char* location, dt_list* packages, 
		      int repository_id)
{
  if (packages == NULL) return false;

  sqlite3* db;
  sqlite3_stmt* result = NULL;
  int status;

  status = sqlite3_open (location, &db);
  if (status != SQLITE_OK) return false;

  /* Minimize disk I/O because that can be pretty expensive. */
  sqlite3_exec (db, "PRAGMA synchronous = OFF", 0, 0, 0);
  sqlite3_exec (db, "PRAGMA journal_mode = MEMORY", 0, 0, 0);
  sqlite3_exec (db, "BEGIN TRANSACTION", 0, 0, 0);

  bool has_done_insert = true;
  dt_list* i = packages;
  dt_package* package;
  while (i != NULL)
    {
      package = i->data;
      i = i->next;

      /* Pretend a record for this package already exists and try to update
       * this record. */
      if (has_done_insert)
	sqlite3_finalize (result),
        status = sqlite3_prepare_v2 (db, 
          "UPDATE packages SET description=?, license=?, category=?, homepage=?, "
          "checksum=? WHERE name=? AND repository_id=?", -1, &result, NULL);

      sqlite3_bind_text (result, 1, package->description, -1, NULL);
      sqlite3_bind_text (result, 2, package->license, -1, NULL);
      sqlite3_bind_text (result, 3, package->category, -1, NULL);
      sqlite3_bind_text (result, 4, package->homepage, -1, NULL);
      sqlite3_bind_text (result, 5, package->checksum, -1, NULL);
      sqlite3_bind_text (result, 6, package->name, -1, NULL);
      sqlite3_bind_int (result, 7, repository_id);

      /* Execute the query. */
      status = sqlite3_step (result);
      if (status != SQLITE_DONE) return false;

      /* When the package isn't in the database, insert it. */
      if (sqlite3_changes (db) > 0)
	{
	  has_done_insert = false;
	  sqlite3_clear_bindings (result);
	  sqlite3_reset (result);
	  continue;
	}
      else
	has_done_insert = true;

      /* Set the INSERT query instead of the UPDATE query. */
      sqlite3_finalize (result);
      status = sqlite3_prepare_v2 (db, 
        "INSERT INTO packages (repository_id, availability, name, description, "
	"license, category, homepage, checksum) VALUES (?, ?, ?, ?, ?, ?, ?, ?)",
	-1, &result, NULL);

      /* Bind the values to the parameters. Apparantly the first index is 1. */
      sqlite3_bind_int (result, 1, repository_id);
      sqlite3_bind_int (result, 2, package->availability);
      sqlite3_bind_text (result, 3, package->name, -1, NULL);
      sqlite3_bind_text (result, 4, package->description, -1, NULL);
      sqlite3_bind_text (result, 5, package->license, -1, NULL);
      sqlite3_bind_text (result, 6, package->category, -1, NULL);
      sqlite3_bind_text (result, 7, package->homepage, -1, NULL);
      sqlite3_bind_text (result, 8, package->checksum, -1, NULL);

      /* Execute the query. */
      status = sqlite3_step (result);
      if (status != SQLITE_DONE) return false;

      /* Prepare the query to be re-executed. */
      sqlite3_clear_bindings (result);
      sqlite3_reset (result);

      if (package->files != NULL)
	db_files_add_list (location, package->files);
    }

  sqlite3_finalize (result); 

  /* Commit the changes to the database. */
  sqlite3_exec (db, "END TRANSACTION", 0, 0, 0);

  sqlite3_close(db);
  return true;
}

bool
db_packages_get_by_custom_filter (const char* location, const char* filter,
			       dt_list** packages)
{
  if (packages == NULL) return false;

  /* Allocate memory for a database connection handler, a result object and 
   * an integer for the status. */
  sqlite3* db;
  sqlite3_stmt *result;
  int status;

  /* Open the database connection. */
  status = sqlite3_open (location, &db);
  if (status != SQLITE_OK) return false;

  size_t query_len = 135 + strlen (filter);
  char* query = calloc (1, query_len + 1);
  strcpy (query,
    "SELECT id, repository_id, name, description, license, category, homepage, "
    "availability, created_at FROM packages ");
  strcat (query, filter);
  strcat (query, " ORDER BY id DESC");

  status = sqlite3_prepare_v2 (db, query, -1, &result, NULL);

  if (status == SQLITE_OK)
    while (sqlite3_step (result) == SQLITE_ROW)
      {
	dt_package* item = calloc (1, sizeof (dt_package));
	if (item == NULL) continue;

	char* buffer;

	item->id = sqlite3_column_int (result, 0);
	item->repository_id = sqlite3_column_int (result, 1);

	buffer = (char *)sqlite3_column_text (result, 2);
	if (buffer != NULL && strlen (buffer) > 1)
	  item->name = calloc (1, strlen (buffer) + 1),
	  item->name = strcpy (item->name, buffer);

	buffer = (char *)sqlite3_column_text (result, 3);
	if (buffer != NULL && strlen (buffer) > 0)
	  item->description = calloc (1, strlen (buffer) + 1),
	  item->description = strcpy (item->description, buffer);

	buffer = (char *)sqlite3_column_text (result, 4);
	if (buffer != NULL && strlen (buffer) > 0)
	  item->license = calloc (1, strlen (buffer) + 1),
	  item->license = strcpy (item->license, buffer);

	buffer = (char *)sqlite3_column_text (result, 5);
	if (buffer != NULL && strlen (buffer) > 0)
	  item->category = calloc (1, strlen (buffer) + 1),
          item->category = strcpy (item->category, buffer);

	buffer = (char *)sqlite3_column_text (result, 6);
	if (buffer != NULL && strlen (buffer) > 0)
	  item->homepage = calloc (1, strlen (buffer) + 1),
	  item->homepage = strcpy (item->homepage, buffer);

	item->availability = sqlite3_column_int (result, 7);

	buffer = (char *)sqlite3_column_text (result, 8);
	if (buffer != NULL && strlen (buffer) > 0)
	  item->created_at = calloc (1, strlen (buffer) + 1),
          item->created_at = strcpy (item->created_at, buffer);

	*packages = dt_list_prepend (*packages, item);
      }

  sqlite3_finalize (result);
  sqlite3_close (db);

  free (query);
  return true;  
}


bool
db_packages_get_all (const char* location, dt_list** packages)
{
  return db_packages_get_by_custom_filter (location, "", packages);
}

bool
db_packages_get_by_repository (const char* location, const char* repository,
			       dt_list** packages)
{
  bool return_val;
  char* filter = calloc (1, 54 + strlen (repository));
  if (filter == NULL) return false;
  sprintf (filter, "WHERE ( SELECT id FROM repositories WHERE domain=\"%s\" )", repository);
  return_val = db_packages_get_by_custom_filter (location, filter, packages);
  free (filter);
  return return_val;
}

bool
db_packages_get_by_keyword (const char* location, const char* keyword,
			    dt_list** packages)
{
  bool return_val;
  char* filter = calloc (1, 110 + strlen (keyword) * 2);
  if (filter == NULL) return false;
  sprintf (filter, "WHERE name LIKE\"%%%s%%\" OR description LIKE \"%%%s%%\" "
	           "OR category LIKE \"%%%s%%\" OR license LIKE \"%%%s%%\"", 
	   keyword, keyword, keyword, keyword);
  return_val = db_packages_get_by_custom_filter (location, filter, packages);
  free (filter);
  return return_val;
}
