/*
 * clangsay -  The classic cowsay program, written in C.
 *
 * file.c
 *
 * Copyright (c) 2015 sasairc
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar.HocevarHocevar See the COPYING file or http://www.wtfpl.net/
 * for more details.
 */

#include "./file.h"
#include "./memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/select.h>

int p_count_file_lines(char** buf)
{
    if (buf == NULL)
        return -1;

    int     i   = 0;

    while (*(buf + i) != NULL)
        i++;

    return i;
}

int p_read_file_char(char*** dest, int t_lines, size_t t_length, FILE* fp, int chomp)
{
    if (t_lines <= 0 || t_length <= 0 || fp == NULL)
        return -1;

    int     x       = 0,
            y       = 0,
            c       = 0;

    size_t  lines   = t_lines,
            length  = t_length,
            tmplen  = 0;

    char*   str     = NULL,
        **  buf     = NULL;

    if ((str = (char*)
                neo_malloc(sizeof(char) * t_length, NULL)) == NULL)
        return -1;

    if ((buf = (char**)
                neo_malloc(sizeof(char*) * t_lines, NULL)) == NULL)
        goto ERR;

    while ((c = fgetc(fp)) != EOF) {
        switch (c) {
            case    '\n':
                if (chomp > 0)
                    *(str + x) = '\0';
                else
                    *(str + x) = c;

                tmplen = strlen(str);
                /* reallocate array of Y coordinate */
                if (y == (lines - 1)) {
                    lines += t_lines;
                    if ((buf = (char**)
                                neo_realloc(buf, sizeof(char*) * lines, NULL)) == NULL)
                        goto ERR;
                }
                /* allocate array for X coordinate */
                if ((*(buf + y) = (char*)
                            neo_malloc(sizeof(char) * (tmplen + 1), NULL)) == NULL)
                    goto ERR;

                /* copy, str to buffer */
                memcpy(*(buf + y), str, tmplen);
                *(*(buf + y) + tmplen) = '\0';
                /* initialize temporary buffer */
                memset(str, '\0', length);
                x = tmplen = 0;
                y++;
                break;
            default:
                /* reallocate temporary array */
                if (x == (length - 1)) {
                    length += t_length;
                    if ((str = (char*)
                                neo_realloc(str, sizeof(char) * length, NULL)) == NULL)
                        goto ERR;
                }
                *(str + x) = c;
                x++;
                continue;
        }
    }

    /* \n -{data}- EOF */
    if (x > 0) {
        /* remove lf? */
        if (chomp > 0)
            *(str + x) = '\0';
        else
            *(str + x) = c;

        tmplen = strlen(str);
        /* reallocate array of Y coordinate */
        if (y == (lines - 1)) {
            lines += t_lines;
            if ((buf = (char**)
                        neo_realloc(buf, sizeof(char*) * lines, NULL)) == NULL)
                goto ERR;
        }
        /* allocate array for X coordinate */
        if ((*(buf + y) = (char*)
                    neo_malloc(sizeof(char) * (tmplen + 1), NULL)) == NULL)
            goto ERR;

        /* copy, str to buffer */
        memcpy(*(buf + y), str, tmplen);
        *(*(buf + y) + tmplen - 1) = '\0';
        y++;
    }

    /* no data */
    if (x == 0 && y == 0) {
        if (buf != NULL)
            free(buf);
        if (str != NULL)
            free(str);

        return 0;
    }
    *(buf + y) = NULL;
    free(str);

    *dest = buf;

    return y;

ERR:
    lines   -= t_lines;
    length  -= t_length;

    if (buf != NULL) {
        while (lines >= 0) {
            if (*(buf + lines) != NULL)
                free(*(buf + lines));

            lines--;
        }
        free(buf);
    }
    if (str != NULL)
        free(str);

    return -3;
}

int watch_fd(int fd, long timeout)
{
    fd_set  fdset;

    struct  timeval tm;

    FD_ZERO(&fdset);
    FD_SET(fd, &fdset);

    tm.tv_sec = tm.tv_usec = timeout;

    return select(fd + 1, &fdset, NULL, NULL, &tm);
}
