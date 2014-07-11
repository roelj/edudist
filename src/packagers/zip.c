#include "zip.h"
#include <zip.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

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
  mode_t permissions = S_IRWXU | S_IRWXG | S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
  mkdir (destination, permissions); 

  int i = 0;
  for (; i < entries; i++)
    {
      struct zip_stat st;
      zip_stat_init (&st);
      if (zip_stat_index (file, i, 0, &st) == -1) return 0;

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

      output_path = strcat (output_path, st.name);

      FILE* output = fopen (output_path, "wb");
      if (output == NULL) return 0;

      fwrite (contents, 1, st.size, output);
      fclose (output);

      free (contents);
      free (output_path);
    }

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
       * processed normally. */
      if (entry->d_type == DT_DIR) 
	{
	  zip_dir_add (zip, entry->d_name, ZIP_FL_ENC_GUESS);

	  size_t name_len = strlen (entry->d_name) + strlen (root) + 1;
	  char* name = calloc (1, name_len + 1);
	  snprintf (name, name_len, "%s%s/", root, entry->d_name);

	  packagers_zip_pack_directory (zip, dir, name);

	  free (name);
	}
      else
	{
	  char* name = calloc (1, strlen (entry->d_name) + strlen (dir) + 2);
	  if (name == NULL) return 0;

	  name = strncpy (name, dir, strlen (dir));
	  if (name[strlen (dir) - 1] != '/') name = strcat (name, "/");
	  name = strcat (name, entry->d_name);

	  struct zip_source* current_file = zip_source_file (zip, name, 0, 0);
	  zip_file_add (zip, name + strlen (root), current_file, ZIP_FL_OVERWRITE);

	  free (name);
	}
    }

  closedir (directory);
  return 1;
}

int
packagers_zip_pack (const char* location, const char* destination)
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

  /* The files are now packed in the ZIP file. Close it and return. */
  if (zip_close (file) == -1) return 0;

  return 1;
}
