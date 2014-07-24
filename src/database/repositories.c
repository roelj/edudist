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
#include <stdlib.h>
#include <string.h>

/*----------------------------------------------------------------------------.
 | DB_REPOSITORIES_ADD                                                        |
 '----------------------------------------------------------------------------*/
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
    "UPDATE repositories SET name=?, is_enabled=1 WHERE domain=?", 
    -1, &result, NULL);

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
        "INSERT INTO repositories (name, domain, is_enabled) VALUES (?, ?, 1)",
        -1, &result, NULL);

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
	  status = sqlite3_prepare_v2 (db,
            "SELECT id FROM repositories WHERE domain=?", -1, &result, NULL);

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

/*----------------------------------------------------------------------------.
 | DB_REPOSITORIES_DISABLE                                                    |
 '----------------------------------------------------------------------------*/
bool
db_repositories_disable (const char* location, const char* domain)
{
  sqlite3* db;
  sqlite3_stmt* result;
  int status;

  status = sqlite3_open (location, &db);
  if (status != SQLITE_OK) return false;

  /* Pretend a record for this domain already exists and try to update
   * this record. */
  status = sqlite3_prepare_v2 (db,
    "UPDATE repositories SET is_enabled=0 WHERE domain=?", -1, &result, NULL);

  /* Bind the values to the parameters. */
  sqlite3_bind_text (result, 1, domain, -1, NULL);

  /* Execute the query. */
  status = sqlite3_step (result);
  if (status != SQLITE_DONE) return false;
  
  sqlite3_finalize (result); 

  sqlite3_close(db);
  return true;
}

/*----------------------------------------------------------------------------.
 | DB_REPOSITORIES_HANDLE_STEP                                                |
 '----------------------------------------------------------------------------*/
static dt_repository*
db_repositories_handle_step (sqlite3_stmt* result)
{
  /* Allocate memory for the package. */
  dt_repository* item = calloc (1, sizeof (dt_repository));
  if (item == NULL) return NULL;

  char* buffer;

  item->id = sqlite3_column_int (result, 0);

  buffer = (char *)sqlite3_column_text (result, 1);
  if (buffer != NULL)
    item->name = calloc (1, strlen (buffer) + 1),
    item->name = strcpy (item->name, buffer);

  buffer = (char *)sqlite3_column_text (result, 2);
  if (buffer != NULL)
    item->domain = calloc (1, strlen (buffer) + 1),
    item->domain = strcpy (item->domain, buffer);

  buffer = (char *)sqlite3_column_text (result, 3);
  if (buffer != NULL)
    item->created_at = calloc (1, strlen (buffer) + 1),
    item->created_at = strcpy (item->created_at, buffer);  

  item->is_enabled = sqlite3_column_int (result, 4);

  return item;
}

/*----------------------------------------------------------------------------.
 | DB_REPOSITORIES_GET_BY_CUSTOM_FILTER                                       |
 '----------------------------------------------------------------------------*/
bool
db_repositories_get_by_custom_filter (const char* location, const char* filter,
			       dt_list** repos)
{
  if (repos == NULL) return false;

  /* Allocate memory for a database connection handler, a result object and 
   * an integer for the status. */
  sqlite3* db;
  sqlite3_stmt *result;
  int status;

  /* Open the database connection. */
  status = sqlite3_open (location, &db);
  if (status != SQLITE_OK) return false;

  size_t query_len = 83 + strlen (filter);
  char* query = calloc (1, query_len + 1);
  strcpy (query, "SELECT id, name, domain, created_at, is_enabled FROM repositories ");
  strcat (query, filter);
  strcat (query, " ORDER BY id DESC");

  status = sqlite3_prepare_v2 (db, query, -1, &result, NULL);

  if (status == SQLITE_OK)
    while (sqlite3_step (result) == SQLITE_ROW)
      {
	dt_repository* item = db_repositories_handle_step (result);
	if (item != NULL)
	  *repos = dt_list_prepend (*repos, item);
      }

  sqlite3_finalize (result);
  sqlite3_close (db);

  free (query);
  return true;  
}

/*----------------------------------------------------------------------------.
 | DB_REPOSITORIES_GET_ALL                                                    |
 '----------------------------------------------------------------------------*/
bool
db_repositories_get_all (const char* location, dt_list** repos)
{
  return db_repositories_get_by_custom_filter (location, "", repos);
}
