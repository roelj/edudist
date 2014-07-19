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
#include "sqlite3.h"

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
