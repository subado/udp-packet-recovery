#pragma once

#include <netinet/in.h>
#include <stdint.h>
#include <sys/socket.h>

#include "packet_types.h"

#define PACKET_SIZE 512 // to avoid IP fragmentation
#define PACKET_HEADER_SIZE                                                    \
  (sizeof (packet_num_t) + sizeof (packet_state_t) + sizeof (packet_size_t))
#define PACKET_DATA_SIZE (PACKET_SIZE - PACKET_HEADER_SIZE)
#define PORT_NUM 50002
#define APPROXIMATE_FILE_BUF_SIZE 5242880
#define MOD_FILE_BUF_SIZE_PACKET_DATA_SIZE                                    \
  (APPROXIMATE_FILE_BUF_SIZE % PACKET_DATA_SIZE)
#define FILE_BUF_SIZE                                                         \
  (APPROXIMATE_FILE_BUF_SIZE + PACKET_DATA_SIZE                               \
   - ((MOD_FILE_BUF_SIZE_PACKET_DATA_SIZE > 0)                                \
          ? MOD_FILE_BUF_SIZE_PACKET_DATA_SIZE                                \
          : PACKET_DATA_SIZE))

#define PACKET_DATA_IN_FILE_BUF_COUNT (FILE_BUF_SIZE / PACKET_DATA_SIZE)
#define RECEIVED_PACKET_PRESENCE_SIZE (PACKET_DATA_IN_FILE_BUF_COUNT + 1)

#define MAX(x, y) (x > y) ? x : y
#define MIN(x, y) (x < y) ? x : y

#define MAX_RETRIES 3

#define TIMEOUT_DIVISOR 4
#define DEFAULT_TIMEOUT_SEC 0L
#define DEFAULT_TIMEOUT_NSEC 50000000L

struct packet_t
{
  packet_num_t packet_num;
  packet_size_t size;
  packet_state_t state_num;
  uint8_t data[PACKET_DATA_SIZE];
};
