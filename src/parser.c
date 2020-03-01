#include <stdlib.h>
#include <stdio.h>
#include "parser.h"

void print_ast_rec(parse_node_t *node){

    if(node == NULL) {
        printf("NULL"); 
        return;
    }

    printf("(");

    printf(")");

}

void print_ast(parse_node_t *node){
    print_ast_rec(node);
    printf("\n");
}
