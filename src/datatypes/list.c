#include "list.h"
#include <stdlib.h>
#include <assert.h>

/*----------------------------------------------------------------------------.
 | DT_LIST_APPEND                                                             |
 | This function adds an element to the list.                                 |
 '----------------------------------------------------------------------------*/
dt_list*
dt_list_append (dt_list* list, void* data)
{
  /* Create the first element if the list is empty. */
  if (list == NULL)
    {
      list = malloc (sizeof (dt_list));
      list->next = NULL;
      list->data = data;
    }

  /* Traverse to the last element and add a new one to it. */
  else
    {
      dt_list* last = list;
      while (last->next != NULL)
        last = last->next;

      last->next = malloc (sizeof (dt_list));
      assert (last->next != NULL);

      last->next->data = data;
      last->next->next = NULL;
    }

  return list;
}

/*----------------------------------------------------------------------------.
 | DT_LIST_PREPEND                                                            |
 | This function adds an element to the list.                                 |
 '----------------------------------------------------------------------------*/
dt_list*
dt_list_prepend (dt_list* list, void* data)
{
  dt_list* new_list = malloc (sizeof (dt_list));
  assert (new_list != NULL);

  new_list->next = list;
  new_list->data = data;

  return new_list;
}


/*----------------------------------------------------------------------------.
 | DT_LIST_REMOVE                                                             |
 | This function adds an element to the list.                                 |
 '----------------------------------------------------------------------------*/
dt_list*
dt_list_remove (dt_list* list)
{
  dt_list* next = list->next;
  free (list);
  return next;
}


/*----------------------------------------------------------------------------.
 | DT_LIST_FREE                                                               |
 | This function adds an element to the list.                                 |
 '----------------------------------------------------------------------------*/
void
dt_list_free (dt_list* list)
{
  while (list != NULL)
    {
      dt_list* prev = list;
      list = list->next;
      free (prev);
      prev = NULL;
    }
}


/*----------------------------------------------------------------------------.
 | DT_LIST_LENGTH                                                             |
 | This function adds an element to the list.                                 |
 '----------------------------------------------------------------------------*/
int
dt_list_length (dt_list* list)
{
  int length = 1;

  /* Return zero when the list is unallocated. */
  if (list == NULL) return 0;

  while (list->next != NULL)
    {
      length++;
      list = list->next;
    }

  return length;
}

/*----------------------------------------------------------------------------.
 | DT_LIST_NEXT                                                               |
 | This function returns the next element in the list.                        |
 '----------------------------------------------------------------------------*/
dt_list*
dt_list_next (dt_list* list)
{
  if (list != NULL)
    return list->next;
  else
    return NULL;
}
