#include <stdio.h>

#include "predecoder.h"



int import_definitions(token_list_t *list, char *filename){
    
    log_info("including file");
    #ifdef DEBUG
        printf(" -> %s\n", filename);
    #else
        printf("\n");
    #endif

    return 0;

}
