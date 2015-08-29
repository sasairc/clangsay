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

#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include "./clangsay.h"

struct  reptarg {
    bool    flag;
    char*   haystack;
    char*   needle;
};
    
/* This functions is required subset.c */
extern FILE* open_file(char* path);
extern int check_file_exists(char* path, char* file);
extern int check_file_stat(char* path);
extern int print_string(int lines, char** str);
extern int print_cow(int lines, char** str, clangsay_t* clsay);
extern int selects_cowfiles(const struct dirent* dir);
extern int list_cowfiles(void);

#endif
