#include "globals.h"
#include "utils/bytes2decimal.h"
#include <string.h>

#include "utils/packet_helpers.h"
#include "utils/socket_helpers.h"

void
read_file_cache (struct packet_t *packet, const uint8_t *file_cache)
{
  packet->size = MIN (PACKET_DATA_SIZE, pending_packets_size
                                            - (packet_to_send.packet_num - 1)
                                                  * PACKET_DATA_SIZE);

  for (size_t i = 0; i < packet->size; ++i)
    {
      packet->data[i]
          = file_cache[i + (packet->packet_num - 1) * PACKET_DATA_SIZE];
    }
}

void
write_file_cache (const struct packet_t *packet, uint8_t *file_cache)
{
  for (size_t i = 0; i < packet->size; ++i)
    {
      file_cache[i + (packet->packet_num - 1) * PACKET_DATA_SIZE]
          = packet->data[i];
    }
}

void
serialize_decimal (const void *field, uint8_t *buf, size_t size, size_t *to)
{
  uintmax_t x = *((uintmax_t *)field);
  switch (size)
    {
    case 2:
      {
        x = htons (x);
        break;
      }
    case 4:
      {
        x = htonl (x);
        break;
      }
    case 8:
      {
        x = htonll (x);
        break;
      }
    }
  decimal2bytes_to (x, buf, size, *to);
  (*to) += size;
}

void
deserialize_decimal (void *field, const uint8_t *buf, size_t size,
                     size_t *from)
{
  bytes2decimal_from ((uintmax_t *)field, buf, size, *from);
  switch (size)
    {
    case 2:
      {
        *((uint16_t *)field) = ntohs (*((uint16_t *)field));
        break;
      }
    case 4:
      {
        *((uint32_t *)field) = ntohl (*((uint32_t *)field));
        break;
      }
    case 8:
      {
        *((uint64_t *)field) = ntohll (*((uint64_t *)field));
        break;
      }
    }
  (*from += size);
}

void
serialize_packet (const struct packet_t *packet, uint8_t *buf)
{
  size_t i = 0;
  serialize_decimal (&packet->packet_num, buf, sizeof (packet_num_t), &i);
  serialize_decimal (&packet->size, buf, sizeof (packet_size_t), &i);
  serialize_decimal (&packet->state_num, buf, sizeof (packet_state_t), &i);
  memcpy (buf + i, packet->data, packet->size);
}

void
deserialize_packet (struct packet_t *packet, const uint8_t *buf)
{
  size_t i = 0;
  deserialize_decimal (&packet->packet_num, buf, sizeof (packet_num_t), &i);
  deserialize_decimal (&packet->size, buf, sizeof (packet_size_t), &i);
  deserialize_decimal (&packet->state_num, buf, sizeof (packet_state_t), &i);

  memcpy (packet->data, buf + i, packet->size);
}
