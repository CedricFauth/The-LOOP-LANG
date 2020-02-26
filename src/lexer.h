#ifndef LEXER_H
#define LEXER_H

token_list_t *open_lexer(int argc, char* argv[]);

int next_statement(token_list_t *list);

void close_lexer(token_list_t *list);

#endif
