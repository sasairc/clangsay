/*
 * clangsay -  The classic cowsay program, written in C.
 *
 * string.h
 *
 * Copyright (c) 2015 sasairc
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar.HocevarHocevar See the COPYING file or http://www.wtfpl.net/
 * for more details.
 */


#ifndef YSTRING_H
#define YSTRING_H

#define WITH_GLIB   /* use glib */
#define LOCALE "ja_JP.UTF-8"

/* This functions is required string.c */
extern int strrep(char* src, char* haystack, char* needle);
extern char* strlion(int argnum, ...);
extern int mbstrlen(char* src);
extern int strunesc(char* src);
extern int strmax(int val, char** src);
extern int strlftonull(char* str);
extern char** str_to_args(char* str);

#endif
