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

// buffers for last received and sent packets
uint8_t received_buf[PACKET_SIZE], buf_to_send[PACKET_SIZE];
// last received and sent packets
struct packet_t received_packet, packet_to_send;

// the index is packet_num
// true indicate that the packet was  received
bool received_packet_presence[RECEIVED_PACKET_PRESENCE_SIZE];

// a number of packets that were received
packet_num_t received_packet_count,
    // a number of packets to be processed
    pending_packets_count,
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

// variables in which socket addresses are stored
struct sockaddr_storage remote_addr, addr;

// ports of socket addresses
in_port_t remote_port, port;

// filename of file used as input or output
const char *filename;

bool changing_remote_state;
// current state of a socket endpoint
packet_state_t state,
    // desired state of a remote socket endpoint
    desired_remote_state;
