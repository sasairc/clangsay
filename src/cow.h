/*
 * clangsay -  The classic cowsay program, written in C.
 *
 * cow.h
 *
 * Copyright (c) 2015 sasairc
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar.HocevarHocevar See the COPYING file or http://www.wtfpl.net/
 * for more details.
 */

#ifndef COW_H
#define COW_H
#ifdef  __cplusplus
extern "C" {
/* __cplusplus */
#endif

/*
 * COWOPT.mode
 */
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

#include <stdio.h>

typedef struct {
    int     mode;
    char*   eye;
    char*   tongue;
} COWOPT;

typedef struct COW {
    FILE*   fp;
    char**  data;
    int     lines;
    int     (*open)(struct COW** cow, char* file);
    int     (*read)(struct COW** cow);
    int     (*print)(struct COW* cow, COWOPT* clsay);
    void    (*release)(struct COW* cow);
} COW;

extern int init_cow(COW** cow);

#ifdef  __cplusplus
}
/* __cplusplus */
#endif
/* COW_H */
#endif
