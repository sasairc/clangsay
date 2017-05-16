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
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
    
void* smalloc(size_t size, const char* fmt, ...)
{
    if (size <= 0)
        NULL;

    void*   buf = NULL;

    va_list args;

    if ((buf = malloc(size)) == NULL) {
        if (fmt != NULL) {
            va_start(args, fmt);
            vfprintf(stderr, fmt, args);
            va_end(args);
        } else {
            fprintf(stderr, "%s: %d: neo_malloc(): %s\n",
                    __FILE__, __LINE__, strerror(errno));
        }

        return NULL;
    }
    memset(buf, '\0', size);

    return buf;
}

void* srealloc(void* ptr, size_t size, const char* fmt, ...)
{
    if (size <= 0)
        return ptr;

    void*   buf = NULL;

    va_list args;

    if ((buf = realloc(ptr, size)) == NULL) {
        if (fmt != NULL) {
            va_start(args, fmt);
            vfprintf(stderr, fmt, args);
            va_end(args);
        } else {
            fprintf(stderr, "%s: %d: neo_realloc(): %s\n",
                    __FILE__, __LINE__, strerror(errno));
        }
        if (ptr != NULL)
            free(ptr);

        return NULL;
    }

    return buf;
}

void free2d(char** buf, int y)
{
    int     i   = 0,
            j   = y - 1;

    while (i <= j) {
        if (*(buf + i) != NULL) {
            free(*(buf + i));
            *(buf + i) = NULL;
        }
        if (*(buf + j) != NULL) {
            free(*(buf + j));
            *(buf + j) = NULL;
        }
        i++;
        j--;
    }
    free(buf);

    return;
}
