#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "token.h"
#include "token.h"

typedef struct parse_node_t parse_node_t;

typedef enum parse_t {
    ST_PROG,
    ST_LOOP,
    ST_ASSIGN,
    EX_BINARY,
    EX_PRIMARY,
    EX_VARIABLE
} parse_t;

typedef struct st_prog_t {

    parse_node_t *current;
    parse_node_t *next;

} st_prog_t;

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

typedef struct ex_var_t {

    char name[4];

} ex_var_t;

typedef struct ex_primary_t {

    u_int32_t value;

} ex_primary_t;


struct parse_node_t {

    parse_t type;

    union
    {
        // statements
        st_prog_t st_prog;
        st_loop_t st_loop;
        st_assign_t st_assign;
        // expressions
        ex_bin_t ex_bin;
        ex_var_t ex_var;
        ex_primary_t ex_primary;
    } value;

};

void print_ast(parse_node_t *node);
void free_ast(parse_node_t *node);

parse_node_t *open_parser();
void close_parser(parse_node_t *node);

void clear_parser(parse_node_t *node);

void parse(token_list_t *list);

#endif
