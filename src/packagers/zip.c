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

#include "zip.h"
#include <zip.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>

int
packagers_zip_unpack (const char* location, const char* destination)
{
  struct zip *file = zip_open (location, 0, NULL);
  if (file == NULL) return 0;

  int entries = zip_get_num_entries (file, 0);
  if (entries < 1) return 0;

  /* Carefully set the permissions of the created directory. We
   * set read and write access for the owner, and read access to 
   * the group this process is being run as. */
  #ifndef WIN32
  mode_t permissions = S_IRWXU | S_IRWXG | S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
  mkdir (destination, permissions); 
  #else
  mkdir (destination);
  #endif

  int i = 0;
  for (; i < entries; i++)
    {
      struct zip_stat st;
      zip_stat_init (&st);
      if (zip_stat_index (file, i, 0, &st) == -1) return 0;

      /* Only extract the content. */
      if (!strcmp (st.name, "metadata.txt")) continue;

      char* contents = calloc (1, st.size + 1);
      if (contents == NULL) return 0;

      struct zip_file* f = zip_fopen (file, st.name, 0);
      if (f == NULL) return 0;

      if (zip_fread (f, contents, st.size) == -1) return 0;
      zip_fclose (f);

      char* output_path = calloc (1, strlen (st.name) + strlen (destination) + 2);
      if (output_path == NULL) return 0;

      output_path = strcpy (output_path, destination);

      if (output_path[strlen (destination) - 1] != '/')
	output_path = strcat (output_path, "/");

      output_path = strcat (output_path, st.name + 8);

      FILE* output = fopen (output_path, "wb");
      if (output == NULL) return 0;

      fwrite (contents, 1, st.size, output);
      fclose (output);

      free (contents);
      free (output_path);
    }

  /* The files are now unpacked in 'destination'. Close it and return. */
  if (zip_close (file) == -1) return 0;

  return 1;
}


static int
packagers_zip_pack_directory (struct zip* zip, const char* root, const char* dir)
{
  /* Open the directory and loop add each file in the directory to the 
   * package file. Don't add hidden files (starting with a dot). */
  DIR* directory;
  if ((directory = opendir (dir)) == NULL) return 0;

  /* Loop over the directory entries. */
  struct dirent* entry;
  while ((entry = readdir (directory)) != NULL)
    {
      /* Don't process files starting with a dot, '.' and '..'. */
      if (entry->d_name[0] == '.') continue;

      /* Directories should be processed recursively, and files should be 
       * processed normally. With MinGW there isn't a 'd_type' in 'dirent'. */
      #ifdef _DIRENT_HAVE_D_TYPE
      if (entry->d_type == DT_DIR) 
	{
      #else
      DIR* test = opendir (entry->d_name);
      if (test != NULL && errno != ENOTDIR)
	{
	  closedir (test);
      #endif
	  /* The files inside the ZIP should be placed in a subdirectory 
	   * called 'content' to separate data from metadata. 'pathname' is the
	   * path inside the ZIP, 'name' is the path on the filesystem. */
	  char pathname[strlen (dir) - strlen (root) + strlen (entry->d_name) + 10];
	  sprintf ((char *)&pathname, "content/%s%s/", dir + strlen (root), entry->d_name);

	  char name[strlen (entry->d_name) + strlen (root) + 2];
	  sprintf ((char *)&name, "%s%s/", dir, entry->d_name);

	  zip_dir_add (zip, (const char*)&pathname, ZIP_FL_ENC_GUESS);
	  packagers_zip_pack_directory (zip, root, name);
	}
      else
	{
	  size_t name_len = strlen (entry->d_name) + strlen (dir) + 2;
	  char name[name_len];
	  memset (&name, 0, name_len);

	  strncpy ((char *)&name, dir, strlen (dir));
	  if (name[strlen (dir) - 1] != '/') strcat ((char *)&name, "/");
	  strncat ((char *)&name, entry->d_name, strlen (entry->d_name));

	  char pathname[name_len + 9];
	  sprintf ((char *)&pathname, "content/%s", name + strlen (root));

	  struct zip_source* current_file = zip_source_file (zip, (char *)&name, 0, 0);
	  zip_file_add (zip, pathname, current_file, ZIP_FL_OVERWRITE);
	}
    }

  closedir (directory);
  return 1;
}

int
packagers_zip_pack (const char* location, const char* destination, dt_package* meta)
{
  /* Make sure the directory path can exists. A common mistake is
   * to provide the package name as first argument and the 
   * directory as second. The destination should not exist, so
   * this could prevent this common error. */
  if (access (location, F_OK) != 0) return 0;

  /* Let's create the package file. It's just a regular ZIP file. */
  struct zip* file = zip_open (destination, ZIP_CREATE | ZIP_TRUNCATE, NULL);
  if (file == NULL) return 0;

  /* The pack_directory function will recursively pack the files and 
   * subdirectories to the ZIP file. It will return 1 if everything went
   * fine and 0 in any other circumstance. */
  if (packagers_zip_pack_directory (file, location, location) != 1) return 0;

  /* Get a string buffer of the metadata. */
  char* output = NULL;
  if (p_package_to_buffer (meta, &output) == 0) return 0;

  /* Prepare the buffer to be added to the ZIP file. */
  struct zip_source* source = zip_source_buffer (file, output, strlen (output), 0);
  if (source == NULL) return 0;

  /* Add the metadata to the ZIP file. */
  if (zip_file_add (file, "metadata.txt", source, ZIP_FL_OVERWRITE) == -1) return 0;

  /* The files are now packed in the ZIP file. Close it and return. */
  if (zip_close (file) == -1) return 0;

  /* Clean up. */
  free (output), output = NULL;

  return 1;
}
