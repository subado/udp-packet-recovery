#pragma once

#include <getopt.h>
#include <stdbool.h>
#include <stddef.h>

void getopt_loop (int argc, char *argv[], const char *short_options,
                  const struct option *long_options,
                  const size_t required_options_size,
                  const char *required_options[required_options_size],
                  const char *help_messages[]);

void
count_required_option (int opt, const size_t required_options_size,
                       const char *required_options[required_options_size],
                       bool required_option_presence[required_options_size],
                       size_t *required_options_count);

void print_help (const struct option *long_options,
                 const size_t required_options_size,
                 const char *required_options[required_options_size],
                 const char *help_messages[]);
