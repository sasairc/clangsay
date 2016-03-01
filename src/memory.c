/*
 * clangsay -  The classic cowsay program, written in C.
 *
 * memory.c
 *
 * Copyright (c) 2015 sasairc
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar.HocevarHocevar See the COPYING file or http://www.wtfpl.net/
 * for more details.
 */

#include "./memory.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

char** malloc2d(int x, int y)
{
    int     i   = 0;

    char**  buf = NULL;

    /* Allocate array for Y coordinate */
    if ((buf = (char**)
                malloc(sizeof(char*) * y)) == NULL)
        return NULL;

    /* Allocate array for X coordinate */
    while (i < y) {
        if ((buf[i] = (char*)
                    malloc(sizeof(char) * x)) == NULL)
            goto ERR;
        i++;
    }

    return buf;

ERR:

    while (i >= 0) {
        if (buf[i] != NULL) {
            free(buf[i]);
            buf[i] = NULL;
        }
        i--;
    }

    return NULL;
}

int init2d(char** buf, int x, int y)
{
    int i = 0;

    /* Initialize each element of array */
    while (i < y) {
        if (buf[i] == NULL) {
            if ((buf[i] = (char*)
                        malloc(sizeof(char) * x)) == NULL)
                return -1;
        }
        memset(buf[i], '\0', x);
        i++;
    }

    return 0;
}
            
void free2d(char** buf, int y)
{
    int i = 0;

    while (i < y) {
        if (buf[i] != NULL) {
            free(buf[i]);
            buf[i] = NULL;
        }
        i++;
    }
    free(buf);

    return;
}
