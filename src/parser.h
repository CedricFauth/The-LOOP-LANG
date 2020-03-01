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
    char name[4];
    //statmnt

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

typedef struct ex_primary_t {

    token_t type;
    
    union{
        char var[4];
        u_int32_t number;
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

program_array_t *parse(token_list_t *list);
void close_parser(program_array_t *prog);


#endif
