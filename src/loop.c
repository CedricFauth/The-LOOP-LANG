#include <stdio.h>
#include <stdlib.h>

#include "token.h"
#include "util/logger.h"
#include "argparser.h"
#include "lexer.h"
#include "parser.h"
#include "environment.h"

#define DEBUG
//#undef DEBUG

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
    
    /*log_warn("testing env\n");
    environment_t *env = create_environment();
    print_environment(env);
    printf("get: %u\n",environment_get(env, "X1"));
    environment_add(env, "X1", 120U);
    printf("get: %u\n",environment_get(env, "X1"));
    environment_add(env, "ASD", 4);
    environment_add(env, "E34", 674355);
    environment_add(env, "E", 34);
    environment_add(env, "F45123", 123);
    printf("asd: %u\n",environment_get(env, "ASD"));
    printf("f43: %u\n",environment_get(env, "F43"));
    printf("e34: %u\n",environment_get(env, "E34"));
    printf("e: %u\n",environment_get(env, "E"));
    environment_add(env, "F43", 987654321);
    printf("f43: %u\n",environment_get(env, "F43"));
    print_environment(env);
    close_environment(env);*/

    close_lexer(token_list);
    close_parser(ast);

    exit(EXIT_SUCCESS);

}
