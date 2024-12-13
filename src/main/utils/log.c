#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "globals.h"

#include "utils/log.h"

void
print_verbose (const char *format, ...)
{
  if (verbose)
    {
      va_list args;
      va_start (args, format);
      vprintf (format, args);
      va_end (args);
    }
}

void
error_exit (const char *format, ...)
{
  va_list args;
  va_start (args, format);
  vprintf (format, args);
  va_end (args);
  exit ((errno == 0) ? 1 : errno);
}

void
perror_exit (const char *s)
{
  perror (s);
  exit (errno);
}
