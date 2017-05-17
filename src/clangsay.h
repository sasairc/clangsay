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

#define CLANGSAY_ALLNO_FLAG \
    0, NULL, NULL, NULL,    \
    {0, NULL}, {0, NULL}

#define MODE_SAY        (1 << 1)
#define MODE_THINK      (1 << 2)
#define MODE_BORG       (1 << 3)
#define MODE_DEAD       (1 << 4)
#define MODE_GREEDY     (1 << 5)
#define MODE_PARANOID   (1 << 6)
#define MODE_STONED     (1 << 7)
#define MODE_TIRED      (1 << 8)
#define MODE_WIRED      (1 << 9)
#define MODE_YOUTHFUL   (1 << 10)
#define MODE_M_EYE      (1 << 11)
#define MODE_M_TONGUE   (1 << 12)
#define MODE_M_FILE     (1 << 13)

struct CLANGSAY_COW_T {
    int     lines;
    char**  data;
};

struct  CLANGSAY_MSG_T {
    int     lines;
    char**  data;
};

typedef struct  CLANGSAY_T {
    int     mode;
    char*   eye;
    char*   tongue;
    char*   file;
    struct CLANGSAY_COW_T   cow;
    struct CLANGSAY_MSG_T   msg;
} clangsay_t;

extern void release(char* path, clangsay_t* clsay);

/* CLANGSAY_H */
#endif
