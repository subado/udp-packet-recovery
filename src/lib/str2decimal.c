#include <ctype.h>
#include <errno.h>
#include <inttypes.h>

#include "str2decimal.h"

int
str2long (uint64_t *out, const char *s, int base)
{
  return str2long_range (out, s, base, 0);
}

int
str2int (uint32_t *out, const char *s, int base)
{
  return str2int_range (out, s, base, 0);
}

int
str2short (uint16_t *out, const char *s, int base)
{
  return str2short_range (out, s, base, 0);
}

int
str2long_range (uint64_t *out, const char *s, int base, uint64_t min_value)
{
  return str2decimal_range ((uintmax_t *)out, s, base, min_value, UINT64_MAX);
}

int
str2int_range (uint32_t *out, const char *s, int base, uint32_t min_value)
{
  return str2decimal_range ((uintmax_t *)out, s, base, min_value, UINT32_MAX);
}

int
str2short_range (uint16_t *out, const char *s, int base, uint16_t min_value)
{
  return str2decimal_range ((uintmax_t *)out, s, base, min_value, UINT16_MAX);
}

int
str2decimal (uintmax_t *out, const char *s, int base)
{
  return str2decimal_range (out, s, base, 0, UINTMAX_MAX);
}

int
str2decimal_range (uintmax_t *out, const char *s, int base,
                   uintmax_t min_value, uintmax_t max_value)
{
  errno = 0;
  char *end;
  if (!isdigit (s[0]))
    {
      errno = EINVAL;
    }
  else
    {
      long l = strtoumax (s, &end, base);

      if (l > max_value || l < min_value)
        {
          errno = ERANGE;
        }
      else if (*end != '\0')
        {
          errno = EINVAL;
        }
      *out = l;
    }

  return errno;
}
