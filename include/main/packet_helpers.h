#pragma once

#include "config.h"

void read_file_cache (struct packet_t *packet, const uint8_t *file_cache);
void write_file_cache (const struct packet_t *packet, uint8_t *file_cache);

void serialize_packet (const struct packet_t *packet, uint8_t *buf);
void deserialize_packet (struct packet_t *packet, const uint8_t *buf);
