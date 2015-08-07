/*
 * clangsay -  The classic cowsay program, written in C.
 *
 * memory.h
 *
 * Copyright (c) 2015 sasairc
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar.HocevarHocevar See the COPYING file or http://www.wtfpl.net/
 * for more details.
 */

#ifndef MEMORY_H
#define MEMORY_H

/* This functions is required memory.c */
extern char** malloc2d(int x, int y);
extern int init2d(char** buf, int x, int y);
extern void free2d(char** buf, int y);

#endif
