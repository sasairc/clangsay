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
#include "./memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int split_env(char* env, env_t **dest)
{
    if (env == NULL)
        return -1;

    int     i       = 0,
            x       = 0,
            y       = 0;

    short   status  = 0;

    size_t  head    = 0,
            tail    = 0;

    env_t*  buf     = NULL;

    if ((buf = (env_t*)
                smalloc(sizeof(env_t), NULL)) == NULL) {
        status = -1; goto ERR;
    }

    i = 0;
    buf->envc = 1;
    while (i < strlen(env)) {
        if (*(env + i) == ':')
            buf->envc++;
        i++;
    }
    if ((buf->envs = (char**)
                smalloc(sizeof(char*) * buf->envc, NULL)) == NULL) {
        status = -2; goto ERR;
    }

    i = x = y = head = tail = 0;
    do {
        if (*(env + tail) == ':' || *(env + tail) == '\0') {
            if ((*(buf->envs + y) = (char*)
                        smalloc(sizeof(char) * (tail - head + 1), NULL)) == NULL) {
                status = -3; goto ERR;
            }

            while (head < tail) {
                *(*(buf->envs + y) + x) = *(env + head);
                head++;
                x++;
            }
            *(*(buf->envs + y) + x + 1) = '\0';
            x = 0;
            head++;
            y++;
        }
        tail++;
    } while (y < buf->envc);

    *dest = buf;

    return 0;

ERR:
    switch (status) {
        case    -1:
            break;
        case    -2:
        case    -3:
        default:
            release_env_t(buf);
    }

    return status;
}

void release_env_t(env_t* env)
{
    if (env == NULL)
        return;

    int i   = 0,
        j   = 0;

    j = env->envc - 1;
    while (i <= j) {
        if (*(env->envs + i) != NULL) {
            free(*(env->envs + i));
            *(env->envs + i) = NULL;
        }
        if (*(env->envs + j) != NULL) {
            free(*(env->envs + j));
            *(env->envs + j) = NULL;
        }
        i++;
        j--;
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
