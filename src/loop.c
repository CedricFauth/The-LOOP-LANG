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
#include <stdlib.h>

#include "token.h"
#include "util/logger.h"
#include "argparser.h"
#include "lexer.h"
#include "parser.h"
#include "environment.h"
#include "interpreter.h"

#define DEBUG
#undef DEBUG

int main(int argc, char* argv[]) {

#ifdef DEBUG
    set_log_level(INFO);
#else
    set_log_level(ERROR);
#endif

    token_list_t *token_list = open_lexer(argc, argv);

    log_info("getting statement tokens\n");
    if(get_tokens(token_list) != 0) {
        close_lexer(token_list);
        exit(EXIT_FAILURE);
    }

#ifdef DEBUG
    print_token_list(token_list);
#endif

    log_info("generating AST\n");
    program_array_t *ast;
    if((ast = parse(token_list)) == NULL) {
        close_lexer(token_list);
        close_parser(ast);
        exit(EXIT_FAILURE);
    }

    log_info("printing ast...\n");
#ifdef DEBUG
    print_ast(ast);
#endif
    log_info("interpreting...\n");

    interpret(ast);

    close_lexer(token_list);
    close_parser(ast);

    exit(EXIT_SUCCESS);

}
