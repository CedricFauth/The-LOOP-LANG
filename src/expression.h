#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "token.h"

typedef struct expr_node expr_node;

typedef enum expr_t {
    PROGRAM,
    BINARY,
    PRIMARY,
    VARIABLE
} expr_t;

typedef struct expr_node_list{

    expr_node *head;

} expr_node_list;

typedef struct expr_prog {

    expr_node_list* expr_list;

} expr_prog;

typedef struct expr_bin {
    token_t op;
    expr_node *left;
    expr_node *right;
} expr_bin;

typedef struct expr_var {

    // TODO

} expr_var;

typedef struct expr_primary {

    // TODO

} expr_primary;


struct expr_node {

    expr_t type;

    union
    {
        expr_bin binary;
        expr_primary number;
    };

    expr_node *next;

};

void print_ast(expr_node* node);
void free_ast(expr_node* node);

#endif
