#include <stdio.h>
#include <stdlib.h>

#include "token.h"
#include "util/logger.h"
#include "argparser.h"
#include "lexer.h"
#include "parser.h"

#define DEBUG
//#undef DEBUG

int main(int argc, char* argv[]) {

#ifdef DEBUG
    set_log_level(INFO);
#else
    set_log_level(WARNING);
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
    program_array_t *ast = NULL;
    if((ast = parse(token_list)) == NULL) {
        close_lexer(token_list);
        close_parser(ast);
        exit(EXIT_FAILURE);
    }

    log_info("printing ast...\n");
#ifdef DEBUG
    print_ast(ast);
#endif

    close_lexer(token_list);
    close_parser(ast);

    exit(EXIT_SUCCESS);

}
