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

char** malloc2d(int x, int y)
{
    char**  buf;
    int     i;

    buf = (char**) malloc(sizeof(char*) * y);       /* Allocate array for Y coordinate */
    if (buf == NULL) {
        return NULL;
    }
    for (i = 0; i < y; i++) {
        buf[i] = (char*)malloc(sizeof(char) * x);   /* Allocate array for X coordinate */
    }

    return buf;
}

int init2d(char** buf, int x, int y)
{
    int i, j;

    /* Initialize each element of array */
    for (i = 0; i < y; i++) {
        if (buf[i] == NULL) buf[i] = (char*)malloc(sizeof(char) * x);   /* If memory allocation failure, do retry memory allocation. */
        for (j = 0; j < x; j++) {
            buf[i][j] = ' ';
        }
    }

    return 0;
}
            
void free2d(char** buf, int y)
{
    int i;

    for (i = 0; i < y; i++) {
        if (buf[i] != NULL) {
            free(buf[i]);       /* Memory release the Y coordinate. */
            buf[i] = NULL;
        }
    }
    free(buf);                  /* Release for Memory */

    return;
}
