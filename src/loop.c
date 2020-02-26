#include <stdio.h>

#include "token.h"
#include "util/logger.h"
#include "util/argparser.h"

int main(int argc, char* argv[]){

    argparse(argc, argv);

    printf("filename: %s\n", get_filename());

    int inp_len = get_input_count();
    for(int i = 0; i < inp_len; i++){
        printf("x%d := %u\n", i+1, get_input_value(i));
    }

    token_list_t *list = new_token_list();

    token_append(list, VARNAME, 20000, 'A', 123);

    token_node_t *t = get_token(list, 0);

    printf("%c%u\n", t->value.name[0], t->value.name[1]);

    free_token_list(list);


    return 0;

}
