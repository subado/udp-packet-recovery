#include <stdbool.h>

#include "config.h"
#include "globals.h"

// a flag indicating that the program is running as a client
bool is_client;

// file descriptors
int sfd, fd,
    // flag for the open()
    open_flag;

// buffer for file io
uint8_t file_buf[FILE_BUF_SIZE];

// the number of bytes with which an io operation was performed
ssize_t received_n_bytes, sent_n_bytes;

// last received and sent packets
struct packet_t received_packet, packet_to_send;

// the index is packet_num
// true indicated that the packet was processed
bool received_packet_presence[PACKET_DATA_IN_FILE_BUF_COUNT + 1];

// a number of packets to be processed
PACKET_NUM_TYPE pending_packets_count,
    // an index of file segment
    // the indexing unit is FILE_BUF_SIZE
    file_segment_idx;
long pending_packets_size, file_remaining_size;

// arguments for nanosleep
struct timespec req, rem;

// a socket communication domain
int socket_family;

// a buffer for presentation string of a socket address
char sockaddr_str[SOCKADDR_STR_LEN];

// lengths of socket addresses
socklen_t remote_socklen, socklen;

// pointers to socket addresses
struct sockaddr_storage *remote_addr, *addr;

// variables in which socket addresses are stored
struct sockaddr_in remote_addr_in, addr_in;
struct sockaddr_in6 remote_addr_in6, addr_in6;

// ports of socket addresses
in_port_t remote_port, port;

// filename of file used as input or output
const char *filename;

// current state of a socket endpoint
state_t state,
    // desired next state of a socket endpoint
    next_state,
    // desired state of a remote socket endpoint
    desired_remote_state;
