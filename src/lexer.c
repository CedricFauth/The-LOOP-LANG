#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#include "token.h"
#include "util/logger.h"
#include "util/argparser.h"

static unsigned int inputs_read = 0;
static unsigned int inputs_count = 0;
static unsigned int current_char = 0;
static int statements = 0;
static FILE *fd;
static char *content;

void readfile(){
    
    fseek(fd, 0L, SEEK_END);
    size_t file_size = ftell(fd);
    fseek(fd, 0L, SEEK_SET);

    if((content = (char*)malloc(file_size + 1)) == NULL){
        log_err("Not enougth memory to read file\n");
        return;
    }
    size_t bytes_read = fread(content, sizeof(char), file_size, fd);
    content[bytes_read] = '\0';

}

token_list_t *open_lexer(int argc, char* argv[]){

    argparse(argc, argv);
    inputs_count = get_input_count();
    printf("Input variables detected: %d\n", inputs_count);
    fd = fopen(get_filename(), "rb");
    if(!fd){
        log_err("Could not open program file.\n");
        return NULL;
    }
    readfile();
    return new_token_list();
}

char current(){
    return content[current_char];
}

char next(){
    return content[++current_char];
}

char peek(int n){
    return content[current_char + n];
}

bool is_digit(char c){
    return c >= '0' && c <= '9' ? true : false;
}

bool is_char(char c){
    return c >= 'A' && c <= 'Z' ? true : false; 
}

void skip(unsigned int n){
    current_char += n;
}

bool matches(char* comp){
    int n = strlen(comp);
    if(strncmp(content+current_char+1, comp, n) == 0){
        skip(n);
        return true; 
    }
        
    return false;
}

void number(token_list_t *list){
    int n = 1;
    char buff[32] = {0};
    buff[0] = current();
    while(is_digit(peek(1))){
        buff[n++] = next();
    }
    add_value_token(list, atoi(buff));

}

void word(token_list_t *list){

    if(is_digit(peek(1))){
        //var name
        int n = 1;
        char var[4];
        var[0] = current();
        while(is_digit(peek(1))){
            var[n++] = next();
        }
        add_name_token(list, var, n);
    }else{
        switch (current()){
            case 'L':
                if(matches("OOP")){
                    add_custom_token(list, LOOP);
                    statements++;
                }
                break;
            case 'D':
                if(matches("O")){
                    add_custom_token(list, DO);
                }
                break;
            case 'E':
                if(matches("ND")){
                    add_custom_token(list, END);
                    statements--;
                }
                break;
        }
    }
}

int next_statement(token_list_t *list){

    int counter = 0;

    clear_token_list(list);

    if(inputs_count > inputs_read){

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

    bool end = false;
    while(!end){

        char c = current();

        if(is_digit(c)){
            number(list);
            counter++;
        }else if(is_char(c)){
            word(list);
            counter++;
        }else{
            switch (c) {
                case ' ': break;
                case '+':
                    add_custom_token(list, PLUS);
                    counter++;
                    break;
                case '-':
                    add_custom_token(list, MINUS);
                    counter++;
                    break;
                case ';':
                    add_custom_token(list, SEMICOLON);
                    counter++;
                    if(statements == 0){
                        end = true;
                    }
                    break;
                case ':':
                    if(next() == '=')
                        add_custom_token(list, ASSIGN);
                        counter++;
                    break;
                case '\0':
                    return counter;
            }

        }
        next();

    }
    return 1;
}

void close_lexer(token_list_t *list){
    free_token_list(list);
    fclose(fd);
    free(content);
}
