/*
 * clangsay -  The classic cowsay program, written in C.
 *
 * env.h
 *
 * Copyright (c) 2015 sasairc
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar.HocevarHocevar See the COPYING file or http://www.wtfpl.net/
 * for more details.
 */

#ifndef ENV_H
#define ENV_H
#ifdef  __cplusplus
extern "C" {
/* __cplusplus */
#endif

typedef struct ENV_T {
    int     envc;
    char**  envs;
} env_t;

extern env_t* split_env(char* env);
extern void release_env_t(env_t* env);

#ifdef  __cplusplus
}
/* __cplusplus */
#endif
/* ENV_H */
#endif
