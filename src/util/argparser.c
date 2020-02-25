#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sysexits.h>

#include "logger.h"

char** arguments;
unsigned int input_count = 0;
unsigned int input_length = 0;

void argparse(int argc, char* argv[]){

    if(argc != 3){
        log_err("usage: loop FILE -in=VALUE1,...\n");
        exit(EX_USAGE);
    }

    arguments = argv;

    if(memcmp("-in=", arguments[2], 4)){
        log_err("no input defined: -in=VALUE,...\n");
        exit(EX_USAGE);
    }  

    input_length = strlen(arguments[2]);
    input_count++;
    for(int i = 4; i < input_length; i++){

        if(arguments[2][i] == ','){
            arguments[2][i] = '\0';
            input_count++;
        }
    }
}

char* get_filename() {
    return arguments[1];
}

unsigned int get_input_value(int index){
    int c = 0;
    for(int i = 4; i < input_length; i++){
        if(c == index){
            return atoi(arguments[2]+i);
        }
        if(arguments[2][i] == '\0') c++;
    }
    return 0;
}

unsigned int get_input_count(){
    return input_count;
}
