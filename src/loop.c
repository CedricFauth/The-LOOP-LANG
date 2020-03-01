#include <stdio.h>
#include <stdlib.h>

#include "token.h"
#include "util/logger.h"
#include "argparser.h"
#include "lexer.h"
#include "parser.h"

int main(int argc, char* argv[]) {

    token_list_t *token_list = open_lexer(argc, argv);
    //parse_node_t *parse_nodes = open_parser();

    /*while(next_statement(token_list)) {
        log_info("Getting statement tokens:\n");
        print_token_list(token_list);

        log_info("Generating AST:\n");
        parse(token_list);

        clear_parser(parse_nodes);
        clear_token_list(token_list);
    }*/

    log_info("Getting statement tokens:\n");
    if(get_tokens(token_list) != 0){
        close_lexer(token_list);
        exit(EXIT_FAILURE);
    }
    print_token_list(token_list);

    close_lexer(token_list);

    exit(EXIT_SUCCESS);

}
