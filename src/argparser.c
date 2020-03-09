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

// stores argument array
static char** arguments;
static unsigned int input_count = 0;
static unsigned int input_length = 0;

/**
 * setup function
 * other methods of argparser only work if argparse() ist called before
 * @param argc number of command line arguments
 * @param argv points to command line inputs
 */
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

    // strlen of -in=... variable
    input_length = strlen(arguments[2]);
    input_count++;
    for(int i = 4; i < input_length; i++) {
        // 'cutting' string at ',' and replacing with '\0'
        if(arguments[2][i] == ',') {
            arguments[2][i] = '\0';
            input_count++;
        }
    }
}

/**
 * getter for filename of *.loop source file
 * @return returns pointer to filename
 */
char* get_filename() {
    return arguments[1];
}

/**
 * getter for input values
 * @param index
 * @return value of -in=...,... argument at index  
 */
uint32_t get_input_value(int index) {
    int c = 0;
    // searches for string at index
    for(int i = 4; i < input_length; i++) {
        if(c == index) {
            // returns integer value from string
            return atoi(arguments[2]+i);
        }
        // next word
        if(arguments[2][i] == '\0') c++;
    }
    return 0;
}

/**
 * getter for number of inputs
 * @return number of input values
 */
uint32_t get_input_count() {
    return input_count;
}
