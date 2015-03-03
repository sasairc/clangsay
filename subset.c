/*
 * clangsay -  The classic cowsay program, written in C.
 *
 * subset.c
 *
 * Copyright (c) 2015 sasairc
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar.HocevarHocevar See the COPYING file or http://www.wtfpl.net/
 * for more details.
 */

#include "./config.h"
#include "./clangsay.h"
#include "./subset.h"
#include "./string.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

int print_string(int lines, char** str)
{
    int i, j;
    int len = strmax(lines, str);       /* get max length */

    /* one line */
    fprintf(stdout, " ");
    for (i = 0; i <= len; i++)
        fprintf(stdout, "_");

    if (lines == 1) {
        fprintf(stdout, "\n< %s >\n ", str[0]);
        for (i = 0; i <= len; i++)
            fprintf(stdout, "-");

        fprintf(stdout, "\n");

        return 0;
    }

    /* multi line */
    for (i = 0; i < lines; i++) {
        if (i == 0) {
            fprintf(stdout, "\n/ %s", str[i]);
        } else if (i == (lines -1)) {
            fprintf(stdout, "\\ %s", str[i]);
        } else {
            fprintf(stdout, "| %s", str[i]);
        }

        for (j = 0; j < (len - mbstrlen(str[i])); j++) {
            fprintf(stdout, " ");
        }

        if (i == 0) {
            fprintf(stdout, " \\\n");
        } else if (i == (lines - 1)) {
            fprintf(stdout, " /\n ");
        } else {
            fprintf(stdout, " |\n");
        }
    }
    for (i = 0; i <= len; i++)
        fprintf(stdout, "-");

    fprintf(stdout, "\n");

    return 0;
}

int print_cow(int lines, char** str, clangsay_t* clsay)
{
    int     i;
    bool    block = false;

    /* print cow */
    for (i = 0; i < lines; i++) {
        strrep(str[i], THOUGHTS, DEFAULT_THOUGHTS);
        while (strrep(str[i], "\\\\", "\\") == 0);

        /* replace eyes*/
        if (clsay->eflag == true) {
            strrep(str[i], EYES, clsay->earg);
        } else if (clsay->bflag == true) {
            strrep(str[i], EYES, BORG_EYES);
        } else if (clsay->dflag == true) {
            strrep(str[i], EYES, DEAD_EYES);
        } else if (clsay->gflag == true) {
            strrep(str[i], EYES, GREEDY_EYES);
        } else if (clsay->pflag == true) {
            strrep(str[i], EYES, PARANOID_EYES);
        } else if (clsay->sflag == true) {
            strrep(str[i], EYES, STONED_EYES);
        } else if (clsay->tflag == true) {
            strrep(str[i], EYES, TIRED_EYES);
        } else if (clsay->wflag == true) {
            strrep(str[i], EYES, WIRED_EYES);
        } else if (clsay->yflag == true) {
            strrep(str[i], EYES, YOUTHFUL_EYES);
        } else {
            strrep(str[i], EYES, DEFAULT_EYES);
        }

        /* replace tongue*/
        if (clsay->Tflag == true) {
            strrep(str[i], TONGUE, clsay->Targ);
        } else if (clsay->dflag == true || clsay->sflag == true) {
            strrep(str[i], TONGUE, DEAD_TONGUE);
        } else {
            strrep(str[i], TONGUE, DEFAULT_TONGUE);
        } 

        /* EOC to EOC */
        if (strstr(str[i], "EOC")) {
            block = true;
            continue;
        } else if (strstr(str[i], "EOC") && block == true) {
            block = false;
        }
        if (block == true) {
            fprintf(stdout, "%s\n", str[i]);
        }
    }

    return 0;
}

int selects_cowfiles(const struct dirent* dir)
{
    int     namlen;
    int*    lp;
    char    dotcow[] = {".cow"};

    namlen = strlen(dir->d_name);
    if (namlen < 4) return 0;
    namlen -= 4;    /* offset 4 bytes from end (.cow) */

    lp = (int*)&(dir->d_name[namlen]);

    /*
     * comparison on int
     * true: .cow
     * false: other
     */
    if (*lp == *(int*)&dotcow)
        return 1;

    return 0;
}

int list_cowfiles(void)
{
    int     i;
    int     entry;
    char*   path    = NULL;
    struct  dirent**  list;

    /* catenate file path */
    if ((path = getenv("COWPATH")) == NULL) {
        path = COWPATH;
    }

    /* get file entry and sort */
    if ((entry = scandir(path, &list, selects_cowfiles, alphasort)) == -1) {
        fprintf(stderr, "%s: scandir() failed\n", PROGNAME);

        exit(11);
    }
    for (i = 0; i < entry; i++) {
        fprintf(stdout, "%s\n", list[i]->d_name);
        free(list[i]);
    }
    free(list);

    exit(0);
}