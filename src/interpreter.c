#include <stdlib.h>
#include <stdio.h>

#include "interpreter.h"
#include "util/logger.h"

static environment_t *env;

void execute_program(program_array_t *prog);

void execute_statement(parse_node_t *stmt);
void execute_loop(st_loop_t *loop);
void execute_assign(st_assign_t *as);

uint32_t execute_expression();
uint32_t execute_binary(ex_bin_t *bin);
uint32_t execute_primary(ex_number_t *pri);


void interpret(program_array_t *ast){

    if(!ast){
        log_err("AST is null, interpretation failed\n");
        return;
    }

    env = create_environment();
    if(!env) return;
    
    // start execution
    execute_program(ast);

    print_environment(env);
    printf("\nX0 := %u\n\n", environment_get(env, "X0"));

    close_environment(env);
}

void execute_program(program_array_t *prog){

    unsigned int i = 0;

    parse_node_t *parse_node;
    while((parse_node = program_get(prog, i++)) != NULL){
        //printf("\nNode %d\n", i);
        //print_ast_rec(parse_node);
        execute_statement(parse_node);

    }
    //printf("\n");
}

void execute_statement(parse_node_t *stmt){

    switch (stmt->type)
        {
        case ST_ASSIGN:
            execute_assign(&stmt->value.st_assign);
            break;
        case ST_LOOP:
            execute_loop(&stmt->value.st_loop);
            break;
        default:
            log_err("statement type not found\n");
            break;
        }

}

void execute_loop(st_loop_t *loop){

    uint32_t max = execute_expression(loop->expr);
    //printf("\n\nLOOPVAR: %u\n\n", max);
    
    for(uint32_t i = 0U; i < max; i++){
        execute_program(loop->programs);
    }
}

void execute_assign(st_assign_t *as){

    environment_add(env, as->name, execute_expression(as->expr));

}

uint32_t execute_expression(parse_node_t *expr){

    switch (expr->type)
    {
    case EX_BINARY:
        return execute_binary(&expr->value.ex_bin);
    case EX_PRIMARY:
        return execute_primary(&expr->value.ex_primary);
    default:
        log_err("unknown expression operator\n");
        break;
    }
    return 0;

}

uint32_t execute_binary(ex_bin_t *bin){

    uint32_t v1 = execute_expression(bin->left);
    uint32_t v2 = execute_expression(bin->right);
    switch (bin->op)
    {
    case PLUS:
        return v1 + v2;
    case MINUS:
        if(v1 <= v2) return 0U;
        return v1 - v2;
    default:
        log_err("unknown binary operator\n");
        break;
    }
    return 0U;

}

uint32_t execute_primary(ex_number_t *pri){

    switch (pri->type)
    {
    case VARNAME:
        return environment_get(env, pri->value.var);
    case NUMBER:
        return pri->value.number;
    default:
        log_err("unknown primary operator\n");
        break;
    }
    return 0U;

}
