#include <ctype.h>
#include <errno.h>
#include <inttypes.h>

#include "utils/str2decimal.h"

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
  return str2decimal_range ((uintmax_t *)out, s, base, min_value, UINT64_MAX,
                            sizeof (uint64_t));
}

int
str2int_range (uint32_t *out, const char *s, int base, uint32_t min_value)
{
  return str2decimal_range ((uintmax_t *)out, s, base, min_value, UINT32_MAX,
                            sizeof (uint32_t));
}

int
str2short_range (uint16_t *out, const char *s, int base, uint16_t min_value)
{
  return str2decimal_range ((uintmax_t *)out, s, base, min_value, UINT16_MAX,
                            sizeof (uint16_t));
}

int
str2decimal (uintmax_t *out, const char *s, int base, size_t size)
{
  return str2decimal_range (out, s, base, 0, UINTMAX_MAX, size);
}

int
str2decimal_range (uintmax_t *out, const char *s, int base,
                   uintmax_t min_value, uintmax_t max_value, size_t size)
{
  errno = 0;
  char *end;
  if (!isdigit (s[0]))
    {
      errno = EINVAL;
    }
  else
    {
      uintmax_t x = strtoumax (s, &end, base);

      if (x > max_value || x < min_value)
        {
          errno = ERANGE;
        }
      else if (*end != '\0')
        {
          errno = EINVAL;
        }

      for (size_t i = 0; i < size; ++i)
        {
          *(((uint8_t *)out) + i) = (x >> (i * sizeof (uint8_t) * 8)) & 0xff;
        }
    }

  return errno;
}
