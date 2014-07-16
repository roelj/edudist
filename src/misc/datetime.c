#include "datetime.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

char*
m_current_timestamp ()
{
  char* timestamp = calloc (1, 20);
  if (timestamp == NULL) return NULL;

  time_t rawtime;
  struct tm* timeinfo;
  time (&rawtime);
  timeinfo = localtime (&rawtime);

  snprintf (timestamp, 20, "%04d-%02d-%02d %02d:%02d:%02d", timeinfo->tm_year + 1900,
	    timeinfo->tm_mon, timeinfo->tm_mday, timeinfo->tm_hour, 
	    timeinfo->tm_min, timeinfo->tm_sec);

  return timestamp;
}
