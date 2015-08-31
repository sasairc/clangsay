/*
 * clangsay -  The classic cowsay program, written in C.
 *
 * env.c
 *
 * Copyright (c) 2015 sasairc
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar.HocevarHocevar See the COPYING file or http://www.wtfpl.net/
 * for more details.
 */

#include "./env.h"
#include <stdlib.h>
#include <string.h>

env_t* split_env(char* env)
{
    int     i       = 0,
            x       = 0,
            y       = 0,
            head    = 0,
            tail    = 0;
    env_t*  buf     = NULL;

    if (env == NULL || (buf = (env_t*)malloc(sizeof(env_t))) == NULL) {

        return NULL;
    }

    for (i = 0, buf->envc = 1; i < strlen(env); i++) {
        if (env[i] == ':')
            buf->envc++;
    }
    if ((buf->envs = (char**)malloc(sizeof(char*) * buf->envc)) == NULL) {
        free(buf);

        return NULL;
    }

    i = x = y = head = tail = 0;
    do {
        if (env[tail] == ':' || env[tail] == '\0') {
            buf->envs[y] = (char*)malloc(sizeof(char) * (tail - head + 1));
            while (head < tail) {
                buf->envs[y][x] = env[head];
                head++;
                x++;
            }
            buf->envs[y][x + 1] = '\0';
            x = 0;
            head++;
            y++;
        }
        tail++;
    } while (y < buf->envc);

    return buf;
}

void release_env_t(env_t* env)
{
    int i = 0;

    for (i = 0; i < env->envc; i++) {
        if (env->envs[i] != NULL) {
            free(env->envs[i]);
            env->envs[i] = NULL;
        }
    }
    if (env->envs != NULL) {
        free(env->envs);
        env->envs = NULL;
    }
    if (env != NULL) {
        free(env);
        env = NULL;
    }

    return;
}
