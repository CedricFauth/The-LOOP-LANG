#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#include "token.h"
#include "util/logger.h"
#include "argparser.h"

#define BOLD "\033[1m"
#define ORANGE "\033[33m\033[1m"
#define RED "\033[31m\033[1m"
#define RESET "\033[0m"

static unsigned int inputs_count = 0;
static unsigned int current_char = 0;
static unsigned int line = 1;
static int statements = 0;
static FILE *fd;
static char *content;

void add_name_token(token_list_t *list, char *name, int len) {
    token_list_append(list, VARNAME, 0, name, len, line);
}

void add_value_token(token_list_t *list, uint32_t value) {
    token_list_append(list, NUMBER, value, 0, 0, line);
}

void add_custom_token(token_list_t *list, token_t type) {
    token_list_append(list, type, 0, 0, 0, line);
}

void lex_error() {

    char *i = content + current_char;
    while(*i != '\0' && *i != '\n') {
        i++;

    }
    (*i) = '\0';
    printf("%s[ERROR] %s%sline %d, unknown or not allowed character: %s%s\n", RED, RESET, BOLD, line, content+current_char, RESET);
}

void readfile() {

    fseek(fd, 0L, SEEK_END);
    size_t file_size = ftell(fd);
    fseek(fd, 0L, SEEK_SET);

    if((content = (char*)malloc(file_size + 1)) == NULL) {
        log_err("Not enougth memory to read file\n");
        return;
    }
    size_t bytes_read = fread(content, sizeof(char), file_size, fd);
    content[bytes_read] = '\0';

}

token_list_t *open_lexer(int argc, char* argv[]) {

    argparse(argc, argv);
    inputs_count = get_input_count();
    printf("Input variables detected: %d\n", inputs_count);
    fd = fopen(get_filename(), "rb");
    if(!fd) {
        log_err("Could not open program file.\n");
        return NULL;
    }
    readfile();
    return new_token_list();
}

char current() {
    return content[current_char];
}

char next() {
    return content[++current_char];
}

char peek(int n) {
    return content[current_char + n];
}

bool is_digit(char c) {
    return c >= '0' && c <= '9' ? true : false;
}

bool is_char(char c) {
    return c >= 'A' && c <= 'Z' ? true : false;
}

void skip(unsigned int n) {
    current_char += n;
}

bool matches(char* comp) {
    int n = strlen(comp);
    if(strncmp(content+current_char+1, comp, n) == 0) {
        skip(n);
        return true;
    }

    return false;
}

void number(token_list_t *list) {
    int n = 1;
    char buff[32] = {0};
    buff[0] = current();
    while(is_digit(peek(1))) {
        buff[n++] = next();
    }
    add_value_token(list, atoi(buff));

}

int word(token_list_t *list) {

    if(is_digit(peek(1))) {
        //var name
        int n = 1;
        char var[4];
        var[0] = current();
        while(is_digit(peek(1))) {
            if(n > 2) {
                current_char -=2;
                return 1;
            }
            var[n++] = next();
        }
        add_name_token(list, var, n);
    } else {
        switch (current()) {
        case 'L':
            if(matches("OOP")) {
                add_custom_token(list, LOOP);
                statements++;
            }
            break;
        case 'D':
            if(matches("O")) {
                add_custom_token(list, DO);
            }
            break;
        case 'E':
            if(matches("ND")) {
                add_custom_token(list, END);
                statements--;
            }
            break;
        default:
            return 1;
        }
    }
    return 0;
}

int get_tokens(token_list_t *list) {

    for(int i = 0; i < inputs_count; i++) {

        char var[4];
        snprintf(var, 4, "X%d", i + 1);
        int len = (i == 0 ? 0 : (int)log10(i));
        add_name_token(list, var, len+2);
        add_custom_token(list, ASSIGN);
        add_value_token(list, get_input_value(i));
        add_custom_token(list, SEMICOLON);

    }

    bool end = false;
    while(!end) {

        char c = current();

        if(is_digit(c)) {
            number(list);
        } else if(is_char(c)) {
            if(word(list) != 0) {
                lex_error();
                return 1;
            }
        } else {
            switch (c) {
            case '\n':
                line++;
            case ' ':
            case '\r':
            case '\t':
                break;
            case '+':
                add_custom_token(list, PLUS);
                break;
            case '-':
                add_custom_token(list, MINUS);
                break;
            case ';':
                add_custom_token(list, SEMICOLON);
                break;
            case ':':
                if(next() == '=') {
                    add_custom_token(list, ASSIGN);
                } else {
                    lex_error();
                    return 1;
                }
                break;
            case '\0':
                if(statements != 0) {
                    log_err("not all opened statements are closed.\n");
                    return 1;
                }
                add_custom_token(list, ENDOFFILE);
                end = true;
                break;
            default:
                lex_error();
                return 1;
            }

        }
        next();

    }
    return 0;
}

void close_lexer(token_list_t *list) {
    free_token_list(list);
    fclose(fd);
    free(content);
}
