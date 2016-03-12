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
    NULL, NULL, NULL

typedef int bool;

typedef struct  CLANGSAY_T {
    bool    bflag;
    bool    dflag;
    bool    gflag;
    bool    pflag;
    bool    sflag;
    bool    tflag;
    bool    wflag;
    bool    yflag;
    bool    eflag;
    bool    Tflag;
    bool    fflag;
    bool    lflag;
    bool    syflag; /* say */
    bool    thflag; /* think */
    char*   earg;
    char*   Targ;
    char*   farg;
} clangsay_t;

extern void release(FILE* fp, env_t* envt, char* path, int lines1, char** buf1, int lines2, char** buf2);

/* CLANGSAY_H */
#endif
