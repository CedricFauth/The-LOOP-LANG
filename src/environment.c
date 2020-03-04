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
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "environment.h"
#include "util/logger.h"

environment_t *create_environment(){
    environment_t *list = malloc(sizeof(environment_t));
    if(!list){
        log_err("Could not create environment. Try again.\n");
        return NULL;
    }
    list->head = NULL;
    return list;
}

void environment_add(environment_t *env, char *name, uint32_t val){
    if(!env){
        log_err("environment is null\n");
        return;
    }

    for(env_node_t *node = env->head; node != NULL; node = node->next){
        if(strncmp(name, node->key, 4) == 0){
            log_info("var already exists, changing value\n");
            node->val = val;
            return;
        }
    }

    env_node_t *node = calloc(1, sizeof(env_node_t));
    if(!node){
        log_err("Could not create environment. Try again.\n");
        return;
    }
    strncpy(node->key, name, 3);
    node->key[3] = '\0';
    node->val = val;
    node->next = env->head;
    env->head = node;
}

uint32_t environment_get(environment_t *env, char *name){
    if(!env){
        log_err("environment is null\n");
        return 0;
    }
    bool found = false;
    env_node_t *node;

    for(node = env->head; node != NULL; node = node->next){
        if(strncmp(name, node->key, 4) == 0){
            found = true;
            break;
        }
    }
    if(found) return node->val;
    log_info("var does not exist, creating var\n");
    environment_add(env, name, 0);
    return 0;

}

void print_environment(environment_t *env){
    if(!env){
        log_err("environment is null\n");
        return;
    }
    printf("env {\n");
    for(env_node_t *tmp = env->head; tmp != NULL; tmp = tmp->next){
        printf("\t%s:\t%u\n", tmp->key, tmp->val);
    }
    printf("}\n");
}

void close_environment(environment_t *env){
    if(!env){
        log_err("environment is null\n");
        return;
    }
    env_node_t *node = env->head;
    env_node_t *tmp;
    while(node){
        log_info("freeing env var\n");
        tmp = node;
        node = node->next;
        free(tmp);
    }
    free(env);
}
