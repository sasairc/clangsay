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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>

int check_file_type(char* filename)
{
    int     i,
            c;
    FILE*   fp  = NULL;

    int     rtf[5] = {0x7B, 0x5C, 0x72, 0x74, 0x66};    /* {\rtf is Ritch-test format's header */

    if ((fp = fopen(filename, "rb")) == NULL) {

        return -1;
    }

    while (0 == feof(fp)) {
        c = fgetc(fp);
        if (c == EOF) {             /* plain text */
            break;
        } else if (c <= 8) {        /* binary or Unknown format */
            fclose(fp);

            return 1;
        } else if (c == 0x7B) {     /* ritch text format */
            rewind(fp);
            for (i = 0; i < 5; i++) {
                c = fgetc(fp);
                if (c == rtf[i]) {
                    continue;
                } else {
                    fclose(fp);

                    return 0;
                }
            }
            fclose(fp);

            return 2;
        }
    }
    fclose(fp);

    return 0;
}   

int count_file_lines(FILE* fp)
{
    int i       = 0,
        lines   = 0;

    rewind(fp);     /* seek file-stream to the top */
    while ((i = getc(fp)) != EOF) {
    if (i == '\n')
        lines++;
    }

    return lines;
}

int read_file(int lines, size_t length, char** buf, FILE* fp)
{
    int     i   = 0;
    char*   str = NULL;

    if ((str = (char*)malloc(sizeof(char) * length)) == NULL) { /* allocate buffer */

        return 0;
    }
    while (i <= lines && fgets(str, sizeof(char) * length, fp) != NULL) {
        buf[i] = (char*)malloc(     /* allocate array for X coordinate */
                    (strlen(str) + 1) * sizeof(char)
                );
        if (buf[i] == NULL) {
            free(str);
            
            return 0;
        }
        strcpy(buf[i], str);        /* copy, str to buffer */
        i++;                        /* count line */
    }
    free(str);

    return i;
}

int p_count_file_lines(char** buf)
{
    int i;

    for (i = 0; buf[i] != NULL; i++);

    return i;
}

char** p_read_file_char(int t_lines, size_t t_length, FILE* fp)
{
    int     i       = 0,
            x       = 0,
            y       = 0,
            c       = 0;

    size_t  lines   = t_lines,
            length  = t_length,
            tmplen  = 0;

    char*   str     = (char*)malloc(sizeof(char) * t_length),   /* allocate temporary array */
        **  buf     = (char**)malloc(sizeof(char*) * t_lines);  /* allocate array of Y coordinate */

    if (str == NULL || buf == NULL) {

        return NULL;
    } else if (t_lines == 0 || t_length == 0 || fp == NULL) {
        free(str);
        free(buf);

        return NULL;
    }

    while ((c = fgetc(fp)) != EOF) {
        switch (c) {
            case    '\n':
                str[x] = c;
                tmplen = strlen(str);
                /* reallocate array of Y coordinate */
                if (y == (lines - 1)) {
                    lines += t_lines;
                    if ((buf = (char**)realloc(buf, sizeof(char*) * lines)) == NULL) {

                        goto ERR;
                    }
                }
                /* allocate array for X coordinate */
                if ((buf[y] = (char*)malloc(sizeof(char) * (tmplen + 1))) == NULL) {

                    goto ERR;
                }
                /* copy, str to buffer */
                memcpy(buf[y], str, tmplen + 1);
                for (i = 0; i < length; i++) {
                    str[i] = '\0';      /* refresh temporary array */
                }
                x = tmplen = 0;
                y++;
                break;
            default:
                /* reallocate temporary array */
                if (x == (length - 1)) {
                    length += t_length;
                    if ((str = (char*)realloc(str, length)) == NULL) {

                        goto ERR;
                    }
                }
                str[x] = c;
                x++;
                continue;
        }
    }
    /* no data */
    if (x == 0 && y == 0) {
        buf[y] = NULL;
        free(str);

        return buf;
    }

    if (str[0] != '\0') {
        if (y == (lines - 1)) {
            str[x] = c;
            lines += t_lines;
            if ((buf = (char**)realloc(buf, sizeof(char*) * lines)) == NULL) {

                goto ERR;
            }
        }
        tmplen = strlen(str);
        if ((buf[y] = (char*)malloc(sizeof(char) * (tmplen + 1))) == NULL) {

            goto ERR;
        }
        memcpy(buf[y], str, tmplen + 1);
        y++;
    }
    buf[y] = NULL;
    free(str);

    return buf;


ERR:
    lines   -= t_lines;
    length  -= t_length;

    if (buf != NULL) {
        for (i = 0; i < lines; i++) {
            if (buf[i] != NULL) {
                free(buf[i]);
                buf[i] = NULL;
            }
        }
        free(buf);
    }
    if (str != NULL)
        free(str);

    return NULL;
}

int watch_fd(int fd, long timeout)
{
    fd_set  fdset;
    struct  timeval tm;

    FD_ZERO(&fdset);
    FD_SET(fd, &fdset);

    tm.tv_sec = tm.tv_usec = timeout;

    return select(fd+1, &fdset, NULL, NULL, &tm);
}
