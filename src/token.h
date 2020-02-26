#ifndef TOKEN
#define TOKEN

#include <stdint.h>

typedef enum token_t {

    // keywords
    LOOP, DO, END,
    
    // operators
    ASSIGN, PLUS, MINUS,

    // literals
    NUMBER, VARNAME,

    // other tokens
    SEMICOLON

} token_t;

typedef struct token_node_t {

    token_t type;

    struct token_node_t* next;

    union {
        // when type is number: 32 bit value
        u_int32_t number;
        // when type is var: 8bit char + 8bit number: A0, A12, X4
        u_int8_t name[2];
    } value;

} token_node_t;

typedef struct token_list_t {

    token_node_t *head;
    token_node_t *tail;

} token_list_t;

token_node_t* new_token(token_t type, u_int32_t value, char n1, u_int8_t n2);

token_list_t* new_token_list();

void token_append(token_list_t *list, token_t type, u_int32_t value, char n1, u_int8_t n2);

void print_token_list(token_list_t* list);

void free_token_list(token_list_t* list);

int size(token_list_t* list);

token_node_t* get_token(token_list_t* l, int index);

#endif
