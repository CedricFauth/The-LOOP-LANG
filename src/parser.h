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

#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "token.h"

typedef struct parse_node_t parse_node_t;

typedef enum parse_t {
    ST_LOOP,
    ST_ASSIGN,
    EX_BINARY,
    EX_PRIMARY
} parse_t;

typedef struct program_array_t {

    parse_node_t **nodes;
    unsigned int size;
    unsigned int pos;

} program_array_t;

typedef struct st_loop_t {

    //var
    parse_node_t *expr;
    //statements
    program_array_t *programs;

} st_loop_t;

typedef struct st_assign_t {

    char name[4];
    parse_node_t *expr;

} st_assign_t;

typedef struct ex_bin_t {
    token_t op;
    parse_node_t *left;
    parse_node_t *right;
} ex_bin_t;

typedef struct ex_number_t {

    token_t type;
    
    union{
        char var[4];
        uint32_t number;
    } value;

} ex_number_t;


struct parse_node_t {

    parse_t type;

    union
    {
        // statements
        st_loop_t st_loop;
        st_assign_t st_assign;
        // expressions
        ex_bin_t ex_bin;
        ex_number_t ex_primary;
    } value;

};

void print_ast(program_array_t *prog);
parse_node_t *program_get(program_array_t *prog, unsigned int i);

void print_ast_rec(parse_node_t *node);

program_array_t *parse(token_list_t *list);
void close_parser(program_array_t *prog);


#endif
