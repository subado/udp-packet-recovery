#include "bytes2decimal.h"

void
bytes2decimal (uintmax_t *x, const uint8_t *bytes, size_t n)
{
  return bytes2decimal_from (x, bytes, n, 0);
}

void
bytes2decimal_from (uintmax_t *x, const uint8_t *bytes, size_t n, size_t from)
{
  for (size_t i = 0; i < n; ++i)
    {
      (*x) ^= (*x) & ((uintmax_t)0xff << (i * 8));
      (*x) |= ((uintmax_t)bytes[i + from]) << (i * 8);
    }
}

void
decimal2bytes (uintmax_t x, uint8_t *bytes, size_t n)
{
  decimal2bytes_to (x, bytes, n, 0);
}

void
decimal2bytes_to (uintmax_t x, uint8_t *bytes, size_t n, size_t to)
{
  for (size_t i = 0; i < n; ++i)
    {
      bytes[i + to] = x >> (i * 8);
    }
}
