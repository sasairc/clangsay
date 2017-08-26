/*
 * clangsay -  The classic cowsay program, written in C.
 *
 * subset.h
 *
 * Copyright (c) 2015 sasairc
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar.HocevarHocevar See the COPYING file or http://www.wtfpl.net/
 * for more details.
 */

#ifndef CLANGSAY_SUB_H
#define CLANGSAY_SUB_H

#include "./clangsay.h"
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>

struct  reptarg {
    int     mode;
    char*   haystack;
    char*   needle;
};

extern int open_cowfile(char* path, FILE** fp);
extern int check_cowfile_exists(char* env, char* file, char** dest);
extern int read_string(clangsay_t* clsay, int argc, int optind, char** argv);
extern int read_cowfile(clangsay_t* clsay, FILE* fp);
extern int print_string(clangsay_t* clsay);
extern int print_cow(clangsay_t* clsay);
extern int list_cowfiles(void);

/* CLANGSAY_SUB_H */
#endif
