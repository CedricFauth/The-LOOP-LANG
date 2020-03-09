/*

MIT License

Copyright (c) 2020 Cedric Fauth

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "util/logger.h"
#include "token.h"

/**
 * creates a new token
 * internal function (use token_list_append for adding tokens to a list) 
 */
token_node_t *new_token(token_t type, uint32_t value, char *name, int len, unsigned int line) {

    //allocate new token
    token_node_t *token = malloc(sizeof(token_node_t));
    if(token == NULL) {
        log_err("Could not allocate token memory.");
        return NULL;
    }

    // setup typ, line, next
    token->type = type;
    token->line = line;
    token->next = NULL;

    // if var token
    if(type == VARNAME) {
        if(len > 3) {
            log_warn("Variable name too long\n");
            len = 3;
        }
        // copy string to token node
        memcpy(token->value.name, name, len);
        token->value.name[len] = '\0';
    // if value token
    } else if (type == NUMBER) {
        token->value.number = value;
    }
    // else: token node has no value or name just a token type

    return token;

}

/**
 * creates a new token list
 * @return new tokenlist or null if allocation fails
 */
token_list_t* new_token_list() {
    // allocate a token list element
    token_list_t* list = malloc(sizeof(token_list_t));
    if(list == NULL) {
        // error
        log_err("Could not allocate token_list memory.\n");
        return NULL;
    }
    // setup
    list->head = NULL;
    list->tail = NULL;
    return list;
}

/**
 * appends adds a new node to a token list
 * @param list is the list where the new token is added to
 * @param type is the token type
 * 
 * @param value number (value of token)
 * or
 * @param name pointer to a 4 byte char array (incl. null)
 * @param len length of name array
 * 
 * @param line number of line
 */
void token_list_append(token_list_t *list, token_t type, uint32_t value, char *name, int len, unsigned int line) {

    // if list == NULL
    if(!list) {
        log_err("List is NULL\n");
        return;
    }

    // this method is declared above and creates a new token
    token_node_t *token = new_token(type, value, name, len, line);
    if(token == NULL) return;

    // add the new token node to the end (tail) of the list
    if(list->head == NULL) {
        list->head = token;
        list->tail = token;
    } else {
        list->tail->next = token;
        list->tail = token;
    }
}

/**
 * prints the content of the token list
 * @param list
 */
void print_token_list(token_list_t *list) {

    // null pointer safety check
    if(list == NULL) {
        log_err("Null instead of list.");
        return;
    }

    printf("tok {\n");
    if(list->head != NULL) {
        // node iterates over every token node in the list
        for(token_node_t *node = list->head; node != NULL; node = node->next) {
            printf("\tl=%u\t", node->line);
            // check node type for print decision
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

/**
 * clears all elements in a given list
 * @param list 
 */
void clear_token_list(token_list_t *list) {
    token_node_t *token = list->head;
    token_node_t *tmp;

    while(token != NULL) {
        // copy address of token to tmp
        tmp = token;
        // set token to the next token
        token = token->next;
        // release ressources of token at adress tmp
        free(tmp);
    }
    // set head to NULL for safety reason
    list->head = NULL;

}

/**
 * frees all resources related to the token list
 */
void free_token_list(token_list_t *list) {

    // free all elements
    clear_token_list(list);
    // free list structure
    free(list);
}

/**
 * calculates the size of the list
 * @param list
 * @return number of elements in the list
 */
int size(token_list_t *list) {
    int c = 0;
    // iterate over list and count iterations
    for(token_node_t *node = list->head; node != NULL; node = node->next, c++);
    return c;
}

/**
 * gets a token node from a list
 * @param list
 * @param index index of the element
 * @return requested token node or null
 */
token_node_t *get_token(token_list_t *list, int index) {
    token_node_t *node = list->head;
    // iterate over list until the index matches i or node is NULL
    for(int i = 0; node->next != NULL && i < index; node = node->next, i++);

    return node;

}
