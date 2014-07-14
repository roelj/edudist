#include "files.h"
#include "sqlite3.h"

bool
db_files_add (const char* location, dt_file* file)
{
  sqlite3* db;
  sqlite3_stmt* result;
  int status;

  status = sqlite3_open (location, &db);
  if (status != SQLITE_OK) return false;

  /* By using a prepared statement we avoid SQL injection and double memory
   * allocation of the query string. */
  status = sqlite3_prepare_v2 (db, 
    "INSERT INTO files (package_id, name, type) VALUES (?, ?, ?)", -1, &result, NULL);

  if (status != SQLITE_OK) return false;

  /* Bind the values to the parameters. Apparantly the first index is 1. */
  sqlite3_bind_int (result, 1, file->package_id);
  sqlite3_bind_text (result, 2, file->name, -1, NULL);
  sqlite3_bind_text (result, 3, file->type, -1, NULL);

  /* Execute the query. */
  status = sqlite3_step (result);
  if (status != SQLITE_DONE) return false;

  sqlite3_finalize (result); 
  sqlite3_close(db);

  return true;
}

bool
db_files_add_list (const char* location, dt_list* files)
{
  if (files == NULL) return false;

  sqlite3* db;
  sqlite3_stmt* result;
  int status;

  status = sqlite3_open (location, &db);
  if (status != SQLITE_OK) return false;

  /* Minimize disk I/O because that can be pretty expensive. */
  sqlite3_exec (db, "PRAGMA synchronous = OFF", 0, 0, 0);
  sqlite3_exec (db, "PRAGMA journal_mode = MEMORY", 0, 0, 0);
  sqlite3_exec (db, "BEGIN TRANSACTION", 0, 0, 0);

  /* By using a prepared statement we avoid SQL injection and double memory
   * allocation of the query string. */
  status = sqlite3_prepare_v2 (db, 
    "INSERT INTO files (package_id, name, type) VALUES (?, ?, ?)", -1, &result, NULL);

  if (status != SQLITE_OK) return false;

  dt_list* i = files;
  dt_file* file;
  while (i != NULL)
    {
      file = i->data;

      /* Bind the values to the parameters. Apparantly the first index is 1. */
      sqlite3_bind_int (result, 1, file->package_id);
      sqlite3_bind_text (result, 2, file->name, -1, NULL);
      sqlite3_bind_text (result, 3, file->type, -1, NULL);

      /* Execute the query. */
      status = sqlite3_step (result);
      if (status != SQLITE_DONE) return false;

      /* Prepare the query to be re-executed. */
      sqlite3_clear_bindings (result);
      sqlite3_reset (result);

      i = i->next;
    }

  sqlite3_finalize (result); 

  /* Commit the changes to the database. */
  sqlite3_exec (db, "END TRANSACTION", 0, 0, 0);

  sqlite3_close(db);

  return true;
}
