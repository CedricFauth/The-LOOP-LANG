#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "util/logger.h"
#include "parser.h"

#define BOLD "\033[1m"
#define ORANGE "\033[33m\033[1m"
#define RED "\033[31m\033[1m"
#define RESET "\033[0m"

static token_list_t *tokens;
static int statements;
static unsigned int position;
static bool error;

void parser_error(char *msg){

    printf("%s[ERROR] %s%sline %u, %s%s", RED, RESET, BOLD, 
            get_token(tokens, position)->line, msg, RESET);
    error = true;
}

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

void close_parser(program_array_t *prog);

void free_parse_node(parse_node_t *node){
    if(!node) return;

    switch (node->type)
    {
    case ST_LOOP:
        free_parse_node(node->value.st_loop.expr);
        close_parser(node->value.st_loop.programs);
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


token_t consume(){
    return get_token(tokens, position++)->type;
    /*
    token_node_t *tmp = tokens->head->next;
    token_t tok = tokens->head->type;
    free(tokens->head);
    tokens->head = tmp;
    return tok;*/
}

bool match(token_t type){
    if(get_token(tokens, position)->type == type){
        return true;
    }
    return false;
    /*
    if(tokens->head == NULL)
        return false;
    if(tokens->head->type == type)
        return true;
    return false;*/
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
        num->value.ex_primary.value.number = get_token(tokens, position)->value.number;//tokens->head->value.number;
        consume();
        return num;
    }else if(match(VARNAME)){
        parse_node_t* num = new_parse_node(EX_PRIMARY);
        num->value.ex_primary.type = VARNAME;
        strncpy(num->value.ex_primary.value.var, (char*) get_token(tokens, position)->value.name, 4);
        consume();
        return num;
    }
    
    parser_error("illegal primary expression\n");
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
    strncpy(buff, (char*) get_token(tokens, position)->value.name, 4);
    consume();

    if(match(ASSIGN)){
        log_info("ASSIGN detected\n");
        consume();
    }else{
        parser_error("no ':=' detected\n");
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
        parser_error("missing 'DO' in LOOP ... DO\n");
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

    parser_error("not a valid statement after ';'\n");
    //print_token_list(tokens);
    return NULL;
}

program_array_t *parse(token_list_t *list){
    error = false;
    statements = 0;
    position = 0;

    if(!list) return NULL;
    tokens = list;

    program_array_t *prog_arr = create_program();
    program_add(prog_arr, statement());
    
    while(match(SEMICOLON)){
        consume();
        program_add(prog_arr, statement());
    }

    if(error){
        close_parser(prog_arr);
        return NULL;
    }

    if(!match(ENDOFFILE)){
        parser_error("expected ';' before next statement\n");
        return NULL;
    }

    consume();
    //print_token_list(tokens);
    return prog_arr;
}

void print_ast_rec(parse_node_t *node);

void print_loop(program_array_t *prog){
    
    for(int i = 0; i < prog->pos; i++){
        parse_node_t *tmp = prog->nodes[i];
        if(!tmp) continue;
        printf("\n");
        for(int j = 0; j < statements; j++)
            printf("\t");
        print_ast_rec(tmp);
    }
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
        printf("ASSIGN %s ", node->value.st_assign.name);
        print_ast_rec(node->value.st_assign.expr);
        break;
    case ST_LOOP:
        statements++;
        printf("LOOP ");
        print_ast_rec(node->value.st_loop.expr);
        printf(" DO ");
        print_loop(node->value.st_loop.programs);
        statements--;
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
        parse_node_t *tmp = program_get(prog, i);
        if(!tmp) continue;
        print_ast_rec(tmp);
        printf("\n");
    }
}

void close_parser(program_array_t *prog){
    if(!prog) return;

    for(int i = 0; i < prog->pos; i++){
        parse_node_t *tmp = prog->nodes[i];
        if(!tmp) continue;
        free_parse_node(tmp);
    }
    free(prog->nodes);
    free(prog);
    
    return;
}

