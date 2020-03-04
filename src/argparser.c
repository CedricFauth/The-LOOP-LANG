/*

MIT License

Copyright (c) 2020 Cedric Fauth

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sysexits.h>

#include "argparser.h"
#include "util/logger.h"

char** arguments;
unsigned int input_count = 0;
unsigned int input_length = 0;

void argparse(int argc, char* argv[]) {

    if(argc != 3) {
        log_err("usage: loop FILE -in=VALUE1,...\n");
        exit(EX_USAGE);
    }

    arguments = argv;

    if(memcmp("-in=", arguments[2], 4)) {
        log_err("no input defined: -in=VALUE,...\n");
        exit(EX_USAGE);
    }

    input_length = strlen(arguments[2]);
    input_count++;
    for(int i = 4; i < input_length; i++) {

        if(arguments[2][i] == ',') {
            arguments[2][i] = '\0';
            input_count++;
        }
    }
}

char* get_filename() {
    return arguments[1];
}

uint32_t get_input_value(int index) {
    int c = 0;
    for(int i = 4; i < input_length; i++) {
        if(c == index) {
            return atoi(arguments[2]+i);
        }
        if(arguments[2][i] == '\0') c++;
    }
    return 0;
}

uint32_t get_input_count() {
    return input_count;
}
