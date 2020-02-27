#ifndef ARGPARSER_H
#define ARGPARSER_H

#include <stdint.h>

void argparse(int argc, char* argv[]);

char* get_filename();

u_int32_t get_input_value(unsigned int index);

unsigned int get_input_count();

#endif
