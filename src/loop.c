#include <stdio.h>

#include "token.h"
#include "util/logger.h"
#include "util/argparser.h"
#include "lexer.h"

int main(int argc, char* argv[]) {

    token_list_t *token_list = open_lexer(argc, argv);

    while(next_statement(token_list)){
        printf("Getting token:\n");
        print_token_list(token_list);

    }
    
    close_lexer(token_list);

    return 0;

}
