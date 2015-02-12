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

#define PROGNAME    "clangsay"
#define VERSION     0
#define PATCHLEVEL  0
#define SUBLEVEL    0
#define AUTHOR      "sasairc"
#define MAIL_TO     "sasairc@ssiserver.moe.hm"

#define PROGNAME    "clangsay"
#define BUFLEN      640
#define STLINE      128
#define true        1
#define false       0

/* static variable */
#define EYES            "$eyes"
#define THOUGHTS        "$thoughts"
#define TONGUE          "$tongue"

#define DEFAULT_THOUGHTS    "o"

#define DEFAULT_EYES    "oo"
#define BORG_EYES       "=="
#define DEAD_EYES       "XX"
#define GREEDY_EYES     "$$"
#define PARANOID_EYES   "@@"
#define STONED_EYES     "**"
#define TIRED_EYES      "--"
#define WIRED_EYES      "OO"
#define YOUTHFUL_EYES   ".."

#define DEFAULT_TONGUE  "  "
#define DEAD_TONGUE     " U"

typedef int bool;
typedef struct CLANGSAY_T {
    bool nflag;
    bool Wflag;
    bool bflag;
    bool dflag;
    bool gflag;
    bool pflag;
    bool sflag;
    bool tflag;
    bool wflag;
    bool yflag;
    bool eflag;
    bool Tflag;
    bool fflag;
    bool lflag;
    int  Warg;
    char* earg;
    char* Targ;
    char* farg;
} clangsay_t;

/* This functions is required clangsay.c */
extern int print_string(int lines, char** str);
extern int list_cowfiles(void);
extern int print_usage(void);

#endif
