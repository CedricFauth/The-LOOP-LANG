#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <stdint.h>

typedef struct env_node_t{

    char key[4];
    u_int32_t val;

    struct env_node_t *next;

} env_node_t;

typedef struct environment_t{

    env_node_t *head;

} environment_t;


environment_t *create_environment();
void print_environment(environment_t *env);
void environment_add(environment_t *env, char *name, u_int32_t val);
u_int32_t environment_get(environment_t *env, char *name);
void close_environment(environment_t env);

#endif
