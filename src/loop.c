#include <stdio.h>

#include "util/logger.h"
#include "util/argparser.h"

int main(int argc, char* argv[]){

    argparse(argc, argv);

    int inp_len = get_input_count();

    printf("filename: %s\n", get_filename());

    for(int i = 0; i < inp_len; i++){

        printf("x%d := %u\n", i+1, get_input_value(i));

    }

    return 0;

}
