#include "package.h"
#include <stdlib.h>

void
dt_package_free (dt_package* package)
{
  free (package->name);
  free (package->description);
  free (package->license);
  free (package->category);
  free (package->homepage);
  free (package->created_at);
  free (package->checksum);
  free (package);
}
