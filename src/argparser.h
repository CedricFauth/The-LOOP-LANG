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

/**
 * argparser is used to handle command line argument parsing
 */

#ifndef ARGPARSER_H
#define ARGPARSER_H

#include <stdint.h>

/**
 * setup function
 * other methods of argparser only work if argparse() ist called before
 * @param argc number of command line arguments
 * @param argv points to command line inputs
 */
void argparse(int argc, char* argv[]);

/**
 * getter for filename of *.loop source file
 * @return returns pointer to filename
 */
char* get_filename();

/**
 * getter for input values
 * @param index
 * @return value of -in=...,... argument at index  
 */
uint32_t get_input_value(int index);

/**
 * getter for number of inputs
 * @return number of input values
 */
uint32_t get_input_count();

#endif
