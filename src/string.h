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

#undef  DEBUG
#define WITH_GLIB   /* use glib */
#define WITH_REGEX  /* use regex */
#define LOCALE      ""

extern int strrep(char* src, char* haystack, char* needle);
extern char* strlion(int argnum, ...);
extern int mbstrlen(char* src);
extern int strunesc(char* src);
extern int strmax(int val, char** src);
extern int strlftonull(char* str);
extern char** str_to_args(char* str);
extern char* mbstrtok(char* str, char* delimiter);
extern int trim(char* str);
extern int strcmp_lite(const char* str1, const char* str2);

#ifdef  WITH_REGEX
#include <regex.h>
#ifdef  WITH_GLIB
extern int mbstrlen_with_regex(char* src, regex_t* reg);
extern int strmax_with_regex(int val, char** src, regex_t* reg);
/* WITH_GLIB */
#endif
/* WITH_REGEX */
#endif

/* YSTRING_H */
#endif
