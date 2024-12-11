#pragma once

#include <netinet/in.h>
#include <stdint.h>
#include <sys/socket.h>

typedef enum
{
  STATE_UNSPEC,
  STATE_CHANGING_REMOTE_STATE,
  STATE_SENDING_PACKET_COUNT,
  STATE_RECEIVING_PACKET_COUNT,
  STATE_SENDING_PACKETS,
  STATE_RECEIVING_PACKETS,
  STATE_SENDING_LOST_PACKET_NUMS,
  STATE_RECEIVING_LOST_PACKET_NUMS,
  STATE_COMPLETE,
  STATE_MAX,
} state_t;

#define PACKET_NUM_TYPE uint32_t
#define PACKET_SIZE_TYPE uint16_t
#define PACKET_SIZE 512
#define PACKET_DATA_SIZE                                                      \
  PACKET_SIZE - sizeof (PACKET_NUM_TYPE) - sizeof (state_t)                   \
      - sizeof (PACKET_SIZE_TYPE)
#define PORT_NUM 50002
#define APPROXIMATE_FILE_BUF_SIZE 5242880
#define MOD_FILE_BUF_SIZE_PACKET_DATA_SIZE                                    \
  (APPROXIMATE_FILE_BUF_SIZE % PACKET_DATA_SIZE)
#define FILE_BUF_SIZE                                                         \
  (APPROXIMATE_FILE_BUF_SIZE + PACKET_DATA_SIZE                               \
   - ((MOD_FILE_BUF_SIZE_PACKET_DATA_SIZE > 0)                                \
          ? MOD_FILE_BUF_SIZE_PACKET_DATA_SIZE                                \
          : PACKET_DATA_SIZE))

#define PACKET_DATA_IN_FILE_BUF_COUNT FILE_BUF_SIZE / PACKET_DATA_SIZE

#define MAX(x, y) (x > y) ? x : y
#define MIN(x, y) (x < y) ? x : y

#define TIMEOUT_NS 7500

struct packet_t
{
  PACKET_NUM_TYPE packet_num;
  PACKET_SIZE_TYPE size;
  state_t state_num;
  uint8_t data[PACKET_DATA_SIZE];
};
