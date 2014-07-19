#include "repository.h"
#include "package.h"
#include <stdlib.h>

void
dt_repository_free (dt_repository* repo)
{
  free (repo->name);
  free (repo->domain);
  free (repo->created_at);

  dt_list* p = repo->packages;
  while (p != NULL)
    dt_package_free (p->data),
    p = p->next;

  dt_list_free (repo->packages);
  free (repo);
}
