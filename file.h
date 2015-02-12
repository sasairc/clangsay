/*
 * clangsay -  The classic cowsay program, written in C.
 *
 * file.h 
 *
 * Copyright (c) 2015 sasairc
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar.HocevarHocevar See the COPYING file or http://www.wtfpl.net/
 * for more details.
 */

#ifndef FILE_H
#define FILE_H

#include <stdio.h>

/* This functions is required file.c */
extern int check_file_type(char* filename);
extern int count_file_lines(FILE* fp);
extern int read_file(int lines, size_t length, char** buf, FILE* fp);
extern char* strlion(int argnum, ...);

#endif
