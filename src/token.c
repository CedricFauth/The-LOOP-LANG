#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "util/logger.h"
#include "token.h"

token_node_t *new_token(token_t type, uint32_t value, char *name, int len, unsigned int line) {

    token_node_t *token = malloc(sizeof(token_node_t));
    if(token == NULL) {
        log_err("Could not allocate token memory.");
        return NULL;
    }

    token->type = type;
    token->line = line;
    token->next = NULL;

    if(type == VARNAME) {
        if(len > 3) {
            log_warn("Variable name too long\n");
            len = 3;
        }
        memcpy(token->value.name, name, len);
        token->value.name[len] = '\0';
    } else if (type == NUMBER) {
        token->value.number = value;
    }

    return token;

}

token_list_t* new_token_list() {
    token_list_t* list = malloc(sizeof(token_list_t));
    if(list == NULL) {
        log_err("Could not allocate token_list memory.\n");
        return NULL;
    }
    list->head = NULL;
    list->tail = NULL;
    return list;
}

void token_list_append(token_list_t *list, token_t type, uint32_t value, char *name, int len, unsigned int line) {

    if(!list) {
        log_err("List is NULL\n");
        return;
    }

    token_node_t *token = new_token(type, value, name, len, line);
    if(token == NULL) return;

    if(list->head == NULL) {
        list->head = token;
        list->tail = token;
    } else {
        list->tail->next = token;
        list->tail = token;
    }
}

void print_token_list(token_list_t *list) {

    if(list == NULL) {
        log_err("Null instead of list.");
        return;
    }

    printf("tok {\n");
    if(list->head != NULL) {
        for(token_node_t *node = list->head; node != NULL; node = node->next) {
            printf("\tl=%u\t", node->line);
            switch (node->type) {
            case LOOP:
                printf("[LOOP] ");
                break;
            case DO:
                printf("[DO] ");
                break;
            case END:
                printf("[END] ");
                break;
            case ASSIGN:
                printf("[ASSIGN] ");
                break;
            case PLUS:
                printf("[PLUS] ");
                break;
            case MINUS:
                printf("[MINUS] ");
                break;
            case NUMBER:
                printf("[NUMBER -> %u] ", node->value.number);
                break;
            case VARNAME:
                printf("[VAR -> %s] ", node->value.name);
                break;
            case SEMICOLON:
                printf("[SEMICOLON] " );
                break;
            case ENDOFFILE:
                printf("[EOF] " );
                break;
            default:
                printf("\033[31m[UNDEFINED -> %d] \033[0m", node->type);
                break;
            }
            printf("\n");
        }
    }
    printf("}\n");

}


void clear_token_list(token_list_t *list) {
    token_node_t *token = list->head;
    token_node_t *tmp;

    while(token != NULL) {
        tmp = token;
        token = token->next;
        free(tmp);
    }
    list->head = NULL;

}

void free_token_list(token_list_t *list) {

    clear_token_list(list);

    free(list);
}

int size(token_list_t *list) {
    int c = 0;
    for(token_node_t *node = list->head; node != NULL; node = node->next, c++);
    return c;
}

token_node_t *get_token(token_list_t *list, int index) {

    token_node_t *node = list->head;
    for(int i = 0; node->next != NULL && i < index; node = node->next, i++);

    return node;

}
