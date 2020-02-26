#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#include "token.h"
#include "util/logger.h"
#include "util/argparser.h"

unsigned int inputs_read = 0;
unsigned int inputs_count = 0;
FILE *fd;

token_list_t *open_lexer(int argc, char* argv[]){

    argparse(argc, argv);
    inputs_count = get_input_count();
    printf("Input variables detected: %d\n", inputs_count);
    fd = fopen(get_filename(), "r");
    if(!fd){
        log_err("Could not open program file.\n");
    }
    return new_token_list();

}


int next_statement(token_list_t *list){

    clear_token_list(list);

    if(inputs_count <= inputs_read){
        
        bool statement = false;
        bool end = false;
        while(!end){

            char buff[64] = {0};
            char c = 0;
            unsigned int n = 0;

            if((c = fgetc(fd)) == EOF) break;
            printf("%c\n", c);

            switch (c) {
            case ' ': break;
            case '+':
                add_custom_token(list, PLUS);
                break;
            case '-':
                add_custom_token(list, MINUS);
                break;
            case ';':
                add_custom_token(list, SEMICOLON);
                if(!statement){
                    end = true;
                }
                break;
            
            default:
                break;
            }


        }

        return 0;
        

    }else{

        char var[4];
        snprintf(var, 4, "X%d", inputs_read);
        int len = (inputs_read == 0 ? 0 : (int)log10(inputs_read));
        add_name_token(list, var, len+2);
        add_custom_token(list, ASSIGN);
        add_value_token(list, get_input_value(inputs_read));
        add_custom_token(list, SEMICOLON);

        inputs_read++;

        return 1;
    }

}

void close_lexer(token_list_t *list){
    free_token_list(list);
    fclose(fd);

}

