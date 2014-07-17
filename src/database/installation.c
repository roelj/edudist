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

#include "installation.h"

#include "sqlite3.h"
#include <unistd.h>

bool db_setup (const char* location)
{
  /* Only continue when the file does not exist yet. Otherwise assume the 
   * database has been set up correctly earlier. */
  if (access (location, F_OK) != -1) return false;

  sqlite3 *db;
  int status;

  /* Attempt to open the database at the given location. */
  status = sqlite3_open (location, &db);

  /* If everything works as it's supposed to work, we can create the tables.  */
  if (status == SQLITE_OK)
    {
      /* Minimize disk I/O because that can be pretty expensive. */
      sqlite3_exec (db, "PRAGMA synchronous = OFF", 0, 0, 0);
      sqlite3_exec (db, "PRAGMA journal_mode = MEMORY", 0, 0, 0);
      sqlite3_exec (db, "BEGIN TRANSACTION", 0, 0, 0);

      if (status == SQLITE_OK)
	status = sqlite3_exec (db, 
          "CREATE TABLE repositories ("
	  " id integer PRIMARY KEY AUTOINCREMENT,"
	  " name varchar(255),"
	  " domain varchar(255),"
	  " created_at timestamp DEFAULT (datetime('now','localtime')))", 0, 0, 0);

      if (status == SQLITE_OK)
	status = sqlite3_exec (db,
          "CREATE TABLE packages ("
	  " id integer PRIMARY KEY AUTOINCREMENT,"
	  " repository_id integer,"
	  " name varchar(255),"
	  " description text,"
	  " license varchar(64),"
	  " category varchar(255),"
	  " homepage varchar(255),"
	  " availability integer,"
	  " checksum varchar(32),"
	  " created_at timestamp DEFAULT (datetime('now','localtime')))", 0, 0, 0);

      if (status == SQLITE_OK)
	status = sqlite3_exec (db, 
          "CREATE TABLE files ("
	  " id integer PRIMARY KEY AUTOINCREMENT,"
	  " package_id integer,"
	  " name varchar(255),"
	  " type varchar(32))", 0, 0, 0);

      /* Commit the changes to the database. */
      if (status == SQLITE_OK)
	sqlite3_exec (db, "END TRANSACTION", 0, 0, 0);

      /* Whether we fail or succeed, we have to close the database. */
      sqlite3_close (db);

      /* When the setup was completed successfully, return positively. */
      if (status == SQLITE_OK)
	return true;
    }

  /* Retry if the database is busy. */
  else if (status == SQLITE_BUSY)
    {
      sqlite3_close (db);
      db_setup (location);
    }

  /* Something unexpected happened, close the database. */
  else
    sqlite3_close (db);

  /* If we made it all the way to here, something went wrong. */
  return false;
}
