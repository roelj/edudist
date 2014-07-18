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

#include "configuration.h"
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

bool
db_configuration_set_defaults (const char* location)
{
  sqlite3* db;
  sqlite3_stmt* result;
  int status;

  status = sqlite3_open (location, &db);
  if (status != SQLITE_OK) return false;

  /* Minimize disk I/O because that can be pretty expensive. */
  sqlite3_exec (db, "PRAGMA synchronous = OFF", 0, 0, 0);
  sqlite3_exec (db, "PRAGMA journal_mode = MEMORY", 0, 0, 0);
  sqlite3_exec (db, "BEGIN TRANSACTION", 0, 0, 0);

  /* Empty the table (TRUNCATE is not available in SQLite3). */
  status = sqlite3_exec (db, "DELETE FROM config", 0, 0, 0);

  /* By using a prepared statement we avoid SQL injection and double memory
   * allocation of the query string. */
  if (status == SQLITE_OK)
    status = sqlite3_prepare_v2 (db, 
      "INSERT INTO config (key, value) VALUES (?, ?)", -1, &result, NULL);

  /* Bind the values to the parameters. Apparantly the first index is 1. */
  sqlite3_bind_int (result, 1, KEY_STORAGE_PATH);

  /* Try to set '$HOME/Moefel/' as the default storage path. 
   * When $HOME is not available, fall back to the current directory. */
  char* storage_path = NULL;
  char* home = getenv ("HOME");
  if (home != NULL)
    {
      storage_path = calloc (1, strlen (home) + 9);
      strcpy (storage_path, home);
      strcat (storage_path, "/Moefel/");
      sqlite3_bind_text (result, 2, storage_path, -1, NULL);
    }
  else
    sqlite3_bind_text (result, 2, ".", -1, NULL);

  /* Execute the query. */
  status = sqlite3_step (result);

  /* Commit the changes to the database. */
  if (status == SQLITE_DONE)
    status = sqlite3_finalize (result),
    free (storage_path);

  /* Write changes to the database. */
  if (status == SQLITE_OK) sqlite3_exec (db, "END TRANSACTION", 0, 0, 0);
  sqlite3_close(db);

  if (status != SQLITE_OK) return false;
  return true;
}

bool
db_configuration_set (const char* location, int key, const char* value)
{
  sqlite3* db;
  sqlite3_stmt* result;
  int status;

  status = sqlite3_open (location, &db);
  if (status != SQLITE_OK) return false;

  /* By using a prepared statement we avoid SQL injection and double memory
   * allocation of the query string. */
  status = sqlite3_prepare_v2 (db, 
    "UPDATE config SET value=? WHERE key=?", -1, &result, NULL);

  if (status != SQLITE_OK) return false;

  /* Bind the values to the parameters. Apparantly the first index is 1. */
  sqlite3_bind_text (result, 1, value, -1, NULL);
  sqlite3_bind_int (result, 2, key);

  /* Execute the query. */
  status = sqlite3_step (result);

  sqlite3_finalize (result); 
  sqlite3_close(db);

  if (status != SQLITE_DONE) return false;

  return true;
}

bool
db_configuration_get (const char* location, int key, char** value)
{
  sqlite3* db;
  sqlite3_stmt *result;
  int status;

  status = sqlite3_open (location, &db);
  if (status != SQLITE_OK) return false;

  status = sqlite3_prepare_v2 (db, "SELECT value FROM config WHERE key=?", -1, &result, NULL);
  sqlite3_bind_int (result, 1, key);

  if (status == SQLITE_OK && sqlite3_step(result) == SQLITE_ROW)
    {
      char* column = (char *)sqlite3_column_text (result, 0);
      *value = calloc (1, strlen (column) + 1);
      *value = strcpy (*value, column);
      status = SQLITE_OK;
    }

  sqlite3_finalize (result);
  sqlite3_close (db);

  if (status == SQLITE_OK) return true;

  return false;
}
