#include <stdbool.h>

#include "config.h"
#include "globals.h"

bool is_client; /* a flag indicating that the program is running as a client */

int sfd, fd,   /* socket and file descriptors */
    open_flag; /* flag for the open() */

uint8_t file_buf[FILE_BUF_SIZE]; /* buffer for file io */

ssize_t received_n_bytes, sent_n_bytes; /* the number of bytes with which an io
                                           operation was performed */

uint8_t received_buf[PACKET_SIZE],
    buf_to_send[PACKET_SIZE]; /* buffers for last received and sent packets */

struct packet_t received_packet,
    packet_to_send; /* last received and sent packets */

bool received_packet_presence
    [RECEIVED_PACKET_PRESENCE_SIZE]; /* the index is packet_num true indicate
                                        that the packet was  received */

packet_num_t
    received_packet_count, /* a number of packets that were received */
    pending_packets_count, /* a number of packets to be processed */
    file_segment_idx;      /* an index of file segment the indexing unit is
                              FILE_BUF_SIZE */

long pending_packets_size, file_remaining_size;

struct timespec req, rem; /* arguments for nanosleep */

int socket_family; /* a socket communication domain */

char sockaddr_str[SOCKADDR_STR_LEN]; /* a buffer for presentation string of a
                                        socket address */

socklen_t remote_socklen, socklen; /* lengths of socket addresses */

struct sockaddr_storage remote_addr, addr; /*socket addresses */

in_port_t remote_port, port; /* ports of socket addresses */

const char *filename; /* filename of file used as input or output */

bool changing_remote_state; /*  current state of a socket endpoint */

packet_state_t state,
    desired_remote_state; /* desired state of a remote socket endpoint */

uint8_t icmp_msg_buf[ICMP_MSG_BUF_SIZE]; // buffer for
struct iovec message_header_iov;         /* Data array */
struct msghdr message_header;            /* Message header */
struct icmphdr icmp_header;              /* ICMP header */
