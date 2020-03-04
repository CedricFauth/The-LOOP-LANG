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

#include <stdio.h>
#include "logger.h"

#define BOLD "\033[1m"
#define ORANGE "\033[33m\033[1m"
#define RED "\033[31m\033[1m"
#define RESET "\033[0m"

static LogLevel LEVEL = INFO;

void set_log_level(LogLevel new_level) {
    LEVEL = new_level;
}

void log_info(char* msg) {
    if(LEVEL <= INFO) printf("%s[INFO]%s %s", BOLD, RESET, msg);
}

void log_warn(char* msg) {
    if(LEVEL <= WARNING) printf("%s[WARNING]%s %s",ORANGE, RESET,msg);
}

void log_err(char* msg) {
    if(LEVEL <= ERROR) printf("%s[ERROR] %s%s%s%s",RED, RESET, BOLD, msg, RESET);
}

