/*
 * clangsay -  The classic cowsay program, written in C.
 *
 * clangsay.h
 *
 * Copyright (c) 2015 sasairc
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar.HocevarHocevar See the COPYING file or http://www.wtfpl.net/
 * for more details.
 */

#ifndef CLANGSAY_H
#define CLANGSAY_H

#include "./env.h"
#include <stdio.h>

#define true        1
#define false       0

#define CLANGSAY_ALLNO_FLAG \
    false, false, false, false, false, false, false, false, false, false, false, false, false, false,   \
    NULL, NULL, NULL,   \
    {0, NULL}, {0, NULL}

typedef int bool;

struct CLANGSAY_COW_T {
    int     lines;
    char**  cow;
};

struct  CLANGSAY_MSG_T {
    int     lines;
    char**  msg;
};

typedef struct  CLANGSAY_T {
    bool                    bflag;
    bool                    dflag;
    bool                    gflag;
    bool                    pflag;
    bool                    sflag;
    bool                    tflag;
    bool                    wflag;
    bool                    yflag;
    bool                    eflag;
    bool                    Tflag;
    bool                    fflag;
    bool                    lflag;
    bool                    syflag; /* say */
    bool                    thflag; /* think */
    char*                   earg;
    char*                   Targ;
    char*                   farg;
    struct CLANGSAY_COW_T   cow;
    struct CLANGSAY_MSG_T   msg;
} clangsay_t;

extern void release(env_t* envt, char* path, clangsay_t* clsay);

/* CLANGSAY_H */
#endif
