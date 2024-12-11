#include "bytes2long.h"

long
bytes2long (uint8_t *bytes, ssize_t n)
{
  return bytes2long_from (bytes, n, 0);
}

long
bytes2long_from (uint8_t *bytes, ssize_t n, ssize_t from)
{
  long l = 0;

  for (ssize_t i = 0; i < n; ++i)
    {
      l |= ((long)bytes[i + from]) << (i * 8);
    }

  return l;
}

void
long2bytes (long l, uint8_t *bytes, ssize_t n)
{
  long2bytes_to (l, bytes, n, 0);
}

void
long2bytes_to (long l, uint8_t *bytes, ssize_t n, ssize_t to)
{
  for (ssize_t i = 0; i < n; ++i)
    {
      bytes[i + to] = l >> (i * 8);
    }
}
