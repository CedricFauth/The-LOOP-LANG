#ifndef ARGPARSER_H
#define ARGPARSER_H

#include <stdint.h>

void argparse(int argc, char* argv[]);

char* get_filename();

uint32_t get_input_value(int index);

uint32_t get_input_count();

#endif
