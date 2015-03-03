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
#include "./clangsay.h"

/* This functions is required subset.c */
extern int print_string(int lines, char** str);
extern int print_cow(int lines, char** str, clangsay_t* clsay);
extern int selects_cowfiles(const struct dirent* dir);
extern int list_cowfiles(void);

#endif