#include <stdlib.h>
#include "util/logger.h"
#include "token.h"

token_node_t* new_token(token_t type, u_int32_t value, char n1, u_int8_t n2){

    token_node_t* token = malloc(sizeof(token_node_t));
    if(token == NULL) {
        log_err("Could not allocate token memory.");
        return NULL;
    }

    token->type = type;
    token->next = NULL;

    if(type == VARNAME){
        token->value.name[0] = (u_int8_t) n1;
        token->value.name[1] = n2;
    }else if (type == NUMBER){
        token->value.number = value;
    }

    return token;

}

token_list_t* new_token_list(){
    token_list_t* list = malloc(sizeof(token_list_t));
    if(list == NULL) {
        log_err("Could not allocate token_list memory.\n");
        return NULL;
    }
    list->head = NULL;
    list->tail = NULL;
    return list;
}

void token_append(token_list_t *list, token_t type, u_int32_t value, char n1, u_int8_t n2){
    token_node_t* token = new_token(type, value, n1, n2);
    if(token == NULL) return;

    if(list->head == NULL){
        list->head = token;
        list->tail = token;
    }else{
        list->tail->next = token;
        list->tail = token;
    }
}

void print_token_list(token_list_t *list){

    // TODO

}

void free_token_list(token_list_t *list){
    token_node_t *token = list->head;
    token_node_t *tmp;

    while(token != NULL){
        tmp = token;
        token = token->next;
        free(tmp);
    }

    free(list);
}

int size(token_list_t* list){
    int c = 0;
    for(token_node_t* node = list->head; node != NULL; node = node->next, c++);
    return c;
}

token_node_t* get_token(token_list_t* l, int index){

    token_node_t* node = l->head;
    for(int i = 0; node->next != NULL && i < index; node = node->next, i++);

    return node;

}
