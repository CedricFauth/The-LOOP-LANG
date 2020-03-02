#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "util/logger.h"
#include "parser.h"

typedef struct error_t {
    bool error;
    int line;
    //TODO

} error_t;

static error_t err;
static token_list_t *tokens;

program_array_t *create_program(){

    program_array_t *program = malloc(sizeof(program_array_t));
    program->size = 4;
    program->pos = 0;
    program->nodes = malloc(program->size*sizeof(parse_node_t*));
    return program;
}

void program_add(program_array_t *prog, parse_node_t *node){
    if(prog->pos >= prog->size){
        log_warn("resizing program array\n");
        prog->size *=2;
        prog->nodes = realloc(prog->nodes, prog->size*sizeof(parse_node_t*));
        if(!prog->nodes){
            log_err("resizing program array failed\n");
        }
    }
    prog->nodes[prog->pos++] = node;
}

parse_node_t *program_get(program_array_t *prog, unsigned int i){

    if(i < prog->pos){
        return prog->nodes[i];
    }
    log_err("program_get failed: i too large");
    return NULL;
}

void program_close(program_array_t *prog);

void free_parse_node(parse_node_t *node){

    switch (node->type)
    {
    case ST_LOOP:
        free_parse_node(node->value.st_loop.expr);
        program_close(node->value.st_loop.programs);
        break;
    case ST_ASSIGN:
        free_parse_node(node->value.st_assign.expr);
        break;
    case EX_BINARY:
        free_parse_node(node->value.ex_bin.left);
        free_parse_node(node->value.ex_bin.right);
        break;
    default:
        break;
    }

    free(node);

}

void program_close(program_array_t *prog){
    if (prog == NULL){
        return;
    }

    for(int i = 0; i < prog->pos; i++){
        free_parse_node(prog->nodes[i]);
    }
    free(prog->nodes);
    free(prog);

}

token_t consume(){
    token_node_t *tmp = tokens->head->next;
    token_t tok = tokens->head->type;
    free(tokens->head);
    tokens->head = tmp;
    return tok;
}

bool match(token_t type){

    if(tokens->head == NULL)
        return false;
    if(tokens->head->type == type)
        return true;
    return false;
}

parse_node_t *new_parse_node(parse_t type){
    parse_node_t *node = malloc(sizeof(parse_node_t));
    if(!node) return NULL;
    node->type = type;
    return node;
}

parse_node_t *primary(){
    log_info("entering primary\n");

    if(match(NUMBER)){
        parse_node_t* num = new_parse_node(EX_PRIMARY);
        num->value.ex_primary.type = NUMBER;
        num->value.ex_primary.value.number = tokens->head->value.number;
        consume();
        return num;
    }else if(match(VARNAME)){
        parse_node_t* num = new_parse_node(EX_PRIMARY);
        num->value.ex_primary.type = VARNAME;
        strncpy(num->value.ex_primary.value.var, (char*) tokens->head->value.name, 4);
        consume();
        return num;
    }
    
    log_err("illegal primary expression\n");
    return NULL;
}

parse_node_t *addition(){

    parse_node_t* pri = primary();
    while(match(PLUS) || match(MINUS)){
        token_t tok = consume();
        if(tok == PLUS){
            log_info("PLUS detected\n");
        }else{
            log_info("MINUS detected\n");
        }
        parse_node_t *tmp = pri;
        pri = new_parse_node(EX_BINARY);
        pri->value.ex_bin.left = tmp;
        pri->value.ex_bin.right = primary();
        pri->value.ex_bin.op = tok;

    }
    
    return pri;

}

parse_node_t *expression(){
    log_info("entering expression\n");

    parse_node_t* add = addition();

    return add;
}

parse_node_t *assign(){

    log_info("entering assign\n");

    char buff[4];

    log_info("VAR detected\n");
    strncpy(buff, (char*) tokens->head->value.name, 4);
    consume();

    if(match(ASSIGN)){
        log_info("ASSIGN detected\n");
        consume();
    }else{
        log_err("no ':=' detected\n");
        return NULL;
    }

    parse_node_t *asgn = new_parse_node(ST_ASSIGN);
    strncpy(asgn->value.st_assign.name, buff, 4);
    asgn->value.st_assign.expr = expression();

    return asgn;
}

parse_node_t *statement();

parse_node_t *loop(){

    log_info("LOOP detected\n");
    consume();
    parse_node_t *expr = expression();
    if(match(DO)){
        log_info("DO detected\n");
        consume();
    }else{
        free_parse_node(expr);
        log_err("missing 'DO' in LOOP ... DO\n");
        return NULL;
    }

    program_array_t *statements = create_program();
    program_add(statements, statement());
    while(!match(END) && match(SEMICOLON)){
        consume();
        program_add(statements, statement());
    }
    consume();
    parse_node_t *loop_st = new_parse_node(ST_LOOP);
    loop_st->value.st_loop.expr = expr;
    loop_st->value.st_loop.programs = statements;
    return loop_st;
    
}

parse_node_t *statement(){
    if(match(VARNAME)){
        return assign();
    }else if(match(LOOP)){
        return loop();
    }

    log_err("not a valid statement after ';'\n");
    print_token_list(tokens);
    return NULL;
}

program_array_t *parse(token_list_t *list){
    err.error = false;
    if(!list) return NULL;
    tokens = list;

    program_array_t *prog_arr = create_program();
    program_add(prog_arr, statement());
    
    while(match(SEMICOLON)){
        consume();
        program_add(prog_arr, statement());
    }

    if(!match(ENDOFFILE)){
        log_err("expected ';' before next statement\n");
        return NULL;
    }

    consume();
    
    print_token_list(tokens);
    return prog_arr;
}

void print_ast_rec(parse_node_t *node);

void print_loop(program_array_t *prog){
    
    for(int i = 0; i < prog->pos; i++){
        printf("\n\t");
        print_ast_rec(program_get(prog, i));
    }
    printf("\n");
}

void print_ast_rec(parse_node_t *node){

    if(node == NULL) {
        printf(" NULL"); 
        return;
    }

    printf("(");

    switch (node->type)
    {
    case ST_ASSIGN:
        printf("ASSIGN: %s", node->value.st_assign.name);
        print_ast_rec(node->value.st_assign.expr);
        break;
    case ST_LOOP:
        printf("LOOP ");
        print_ast_rec(node->value.st_loop.expr);
        printf(" DO ");
        print_loop(node->value.st_loop.programs);
        break;
    case EX_BINARY:
        print_ast_rec(node->value.ex_bin.left);
        switch (node->value.ex_bin.op)
        {
        case PLUS:
            printf(" + ");
            break;
        case MINUS:
            printf(" - ");
            break;
        default: break;
        }
        print_ast_rec(node->value.ex_bin.right);
    case EX_PRIMARY:
        switch (node->value.ex_primary.type)
        {
        case VARNAME:
            printf("VAR %s", node->value.ex_primary.value.var);
            break;
        case NUMBER:
            printf("NUM %u", node->value.ex_primary.value.number);
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }

    printf(")");

}

void print_ast(program_array_t *prog){
    for(int i = 0; i < prog->pos; i++){
        printf("Statement %d:\n", i);
        print_ast_rec(program_get(prog, i));
        printf("\n");
    }
}

void close_parser(program_array_t *prog){
    program_close(prog);
    return;
}

