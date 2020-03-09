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


#ifndef TOKEN_H
#define TOKEN_H

#include <stdint.h>

/**
 * All tokens that the lexer uses
 * to generate token lists
 */
typedef enum token_t {

    // keywords
    LOOP, DO, END,

    // operators
    ASSIGN, PLUS, MINUS,

    // literals
    NUMBER, VARNAME,

    // other tokens
    SEMICOLON, ENDOFFILE,

    // tokens for own definitions
    NEW_DEF, CALL_DEF

} token_t;

/**
 * Token Node is a struct that
 * is used as a list element
 */
typedef struct token_node_t {

    token_t type;
    unsigned int line;
    struct token_node_t* next;

    union {
        // call id for new_def and call_def tokens
        uint32_t id;
        // when type is number: 32 bit value
        uint32_t number;
        // when type is var: 8bit char + 8bit number: A0, A12, X4
        uint8_t name[4];
    } value;

} token_node_t;

/**
 * Token list is a list that 
 * stores all tokens as token nodes
 */
typedef struct token_list_t {

    token_node_t *head;
    token_node_t *tail;

} token_list_t;

// methods

/**
 * creates a new token list
 * @return new tokenlist or null if allocation fails
 */
token_list_t* new_token_list();

/**
 * frees all resources related to the token list
 */
void free_token_list(token_list_t *list);

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
void token_list_append(token_list_t *list, token_t type, uint32_t value, 
                        char *name, int len, unsigned int line);

/**
 * calculates the size of the list
 * @param list
 * @return number of elements in the list
 */
int size(token_list_t *list);

/**
 * gets a token node from a list
 * @param list
 * @param index index of the element
 * @return requested token node or null
 */
token_node_t *get_token(token_list_t *list, int index);

/**
 * prints the content of the token list
 * @param list
 */
void print_token_list(token_list_t *list);

#endif
