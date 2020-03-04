#include <stdlib.h>
#include <stdio.h>
#include "interpreter.h"
#include "util/logger.h"

static environment_t *env;

void execute_statement(parse_node_t *stmt);
uint32_t execute_expression();

void interpret(program_array_t *ast){

    if(!ast){
        log_err("AST is null, interpretation failed\n");
        return;
    }

    env = create_environment();
    if(!env) return;

    unsigned int i = 0;
    parse_node_t *parse_node;
    while((parse_node = program_get(ast, i++)) != NULL){
        printf("\nNode %d\n", i);
        print_ast_rec(parse_node);
        
        execute_statement(parse_node);

    }
    printf("\n");

    print_environment(env);
    close_environment(env);
}

void execute_statement(parse_node_t *stmt){

    switch (stmt->type)
        {
        case ST_ASSIGN:
            environment_add(env, stmt->value.st_assign.name, 
                        execute_expression(stmt->value.st_assign.expr));
            break;
        case ST_LOOP:
            //execute_loop_statement(...);
            //TODO
            break;
        default:
            log_err("statement type not found\n");
            break;
        }

}

uint32_t execute_expression(parse_node_t *expr){

    uint32_t v1;
    uint32_t v2;

    switch (expr->type)
    {
    case EX_BINARY:
        v1 = execute_expression(expr->value.ex_bin.left);
        v2 = execute_expression(expr->value.ex_bin.right);
        switch (expr->value.ex_bin.op)
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
        break;

    case EX_PRIMARY:
        switch (expr->value.ex_primary.type)
        {
        case VARNAME:
            return environment_get(env, expr->value.ex_primary.value.var);
        case NUMBER:
            return expr->value.ex_primary.value.number;
        default:
            log_err("unknown primary operator\n");
            break;
        }
        break;

    default:
        log_err("unknown expression operator\n");
        break;
    }
    return 0;

}
