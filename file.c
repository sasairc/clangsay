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

int check_file_type(char* filename)
{
    int     i, c;
    FILE*   fp = NULL;

    int     rtf[5] = {0x7B, 0x5C, 0x72, 0x74, 0x66};    /* {\rtf is Ritch-test format's header */

    fp = fopen(filename, "rb");
    if (fp == NULL) {
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
    int i;
    int lines = 0;

    rewind(fp);     /* seek file-stream to the top */
    while ((i = getc(fp)) != EOF) {
        if (i == '\n')
            lines++;
    }

    return lines;
}

int read_file(int lines, size_t length, char** buf, FILE* fp)
{
    int     i = 0;
    char*   str = (char*)malloc(sizeof(char) * length); /* Allocate buffer */

    while (i <= lines && fgets(str, sizeof(char) * length, fp) != NULL) {
        if (str[strlen(str) - 1] == '\n') { /* Checking string length */
            /* 0: string < BUFLEN */
            str[strlen(str) - 1] = '\0';
            buf[i] = (char*)malloc(         /* Allocate array for X coordinate */
                    (strlen(str) + 1) * sizeof(char)
            );
            strcpy(buf[i], str);            /* Copy, str to buffer */
        } else {
            /* 1: string > BUFLEN */
            free(str);
            return 0;
        }
        i++;
    }
    free(str);

    return i;
}
