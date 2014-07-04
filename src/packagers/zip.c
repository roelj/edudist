#include "zip.h"
#include <zip.h>
#include <stdlib.h>

int
packagers_zip_unpack (const char* location, const char* destination)
{
  struct zip *file = zip_open (location, 0, NULL);
  if (file == NULL) return 0;

  int entries = zip_get_num_entries (file, 0);
  if (entries < 1) return 0;

  int i = 0;
  for (; i < entries; i++)
    {
      struct zip_stat st;
      zip_stat_init (&st);
      zip_stat_index (file, i, 0, &st);

      char* contents = calloc (1, st.size + 1);
      struct zip_file* f = zip_fopen (file, st.name, 0);

      zip_fread (f, contents, st.size);
      zip_fclose (f);

      FILE* output = fopen (st.name, "wb");
      fwrite (contents, 1, st.size, output);
      fclose (output);

      free (contents);
    }

}
