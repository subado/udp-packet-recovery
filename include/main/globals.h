#pragma once

#include <stdbool.h>

#include "config.h"

extern bool is_client;

extern int sfd, fd, open_flag;

extern uint8_t file_buf[FILE_BUF_SIZE];

extern ssize_t received_n_bytes, sent_n_bytes;

extern uint8_t received_buf[PACKET_SIZE], buf_to_send[PACKET_SIZE];
extern struct packet_t received_packet, packet_to_send;

extern bool received_packet_presence[RECEIVED_PACKET_PRESENCE_SIZE];

extern packet_num_t received_packet_count, pending_packets_count,
    file_segment_idx;
extern long pending_packets_size, file_remaining_size;

extern struct timespec req, rem;

extern int socket_family;

#define SOCKADDR_STR_LEN MAX (INET6_ADDRSTRLEN, INET_ADDRSTRLEN)
extern char sockaddr_str[SOCKADDR_STR_LEN];

extern socklen_t remote_socklen, socklen;

extern struct sockaddr_storage remote_addr, addr;

extern in_port_t remote_port, port;

extern const char *filename;

extern bool changing_remote_state;
extern packet_state_t state, desired_remote_state;
