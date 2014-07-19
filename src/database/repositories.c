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

#include "repositories.h"
#include "sqlite3.h"
#include "packages.h"
#include <stddef.h>

bool
db_repositories_add (const char* location, dt_repository* repository)
{
  sqlite3* db;
  sqlite3_stmt* result;
  int status;

  status = sqlite3_open (location, &db);
  if (status != SQLITE_OK) return false;


  /* Pretend a record for this domain already exists and try to update
   * this record. */
  status = sqlite3_prepare_v2 (db,
    "UPDATE repositories SET name=? WHERE domain=?", -1, &result, NULL);

  /* Bind the values to the parameters. */
  sqlite3_bind_text (result, 1, repository->name, -1, NULL);
  sqlite3_bind_text (result, 2, repository->domain, -1, NULL);

  /* Execute the query. */
  status = sqlite3_step (result);
  if (status != SQLITE_DONE) return false;

  /* See whether a row was affected. If not, the record doesn't exist and
   * we should run an INSERT query instead. */
  if (sqlite3_changes (db) == 0)
    {
      /* Clean up the previous prepared statement and its result set. */
      sqlite3_finalize (result);

      /* By using a prepared statement we avoid SQL injection and double memory
       * allocation of the query string. */
      status = sqlite3_prepare_v2 (db, 
        "INSERT INTO repositories (name, domain) VALUES (?, ?)", -1, &result, NULL);

      if (status != SQLITE_OK) return false;

      /* Bind the values to the parameters. Apparantly the first index is 1. */
      sqlite3_bind_text (result, 1, repository->name, -1, NULL);
      sqlite3_bind_text (result, 2, repository->domain, -1, NULL);

      /* Execute the query. */
      status = sqlite3_step (result);
      if (status != SQLITE_DONE) return false;
    }

  sqlite3_finalize (result); 

  /* Add the packages to the database. */
  if (repository->packages != NULL)
    {
      /* In the case of the 'repositories', rowid is the same as id. */
      int id = sqlite3_last_insert_rowid (db);

      /* If no INSERT was done, we need to do a little more work to get the
       * repository ID. */
      if (id == 0)
	{
	  status = sqlite3_prepare_v2 (db, "SELECT id FROM repositories WHERE domain=?", -1, &result, NULL);
	  sqlite3_bind_text (result, 1, repository->domain, -1, NULL);

	  if (status == SQLITE_OK && sqlite3_step(result) == SQLITE_ROW)
	    id = sqlite3_column_int (result, 0);

	  sqlite3_finalize (result);
	}

      db_packages_add_list (location, repository->packages, id);
    }

  sqlite3_close(db);
  return true;
}
