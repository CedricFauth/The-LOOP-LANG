#ifndef TOKEN_H
#define TOKEN_H

#include <stdint.h>

typedef enum token_t {

    // keywords
    LOOP, DO, END,

    // operators
    ASSIGN, PLUS, MINUS,

    // literals
    NUMBER, VARNAME,

    // other tokens
    SEMICOLON, ENDOFFILE

} token_t;

typedef struct token_node_t {

    token_t type;
    unsigned int line;
    struct token_node_t* next;

    union {
        // when type is number: 32 bit value
        uint32_t number;
        // when type is var: 8bit char + 8bit number: A0, A12, X4
        uint8_t name[4];
    } value;

} token_node_t;

typedef struct token_list_t {

    token_node_t *head;
    token_node_t *tail;

} token_list_t;


void token_list_append(token_list_t *list, token_t type, uint32_t value, 
                        char *name, int len, unsigned int line);
token_list_t* new_token_list();
void free_token_list(token_list_t *list);
int size(token_list_t *list);
token_node_t *get_token(token_list_t *list, int index);
void print_token_list(token_list_t *list);

#endif
