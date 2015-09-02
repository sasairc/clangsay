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
#include "./file.h"
#include "./env.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

FILE* open_file(char* path)
{
    FILE*   fp;

    fp = fopen(path, "r");

    /* open after checking file type */
    if (check_file_type(path) == 0) {
        fp = fopen(path, "r");
    } else {
        fprintf(stderr, "%s: %s: unknown file type\n",
                PROGNAME, path);

        return NULL;
    }
    if (fp == NULL) {
        fprintf(stderr, "%s: fp is NULL\n",
                PROGNAME);

        return NULL;
    }

    return fp;
}

int check_file_exists(char* path, char* file)
{
    int     ret     = 0;
    char*   tmp     = NULL;
    DIR*    dp      = NULL;

    struct  stat    st;
    struct  dirent* list;

    if (stat(file, &st) == 0)
        return 1;

    if ((dp = opendir(path)) == NULL)
        return 0;

    if ((tmp = strlion(2, file, ".cow")) == NULL)
        return 0;

    for (list = readdir(dp); list != NULL; list = readdir(dp)) {
        if (strcmp(list->d_name, file) == 0) {
            ret = 2;
            break;
        } else if (strcmp(list->d_name, tmp) == 0) {
            ret = 3;
            break;
        }
    }
    closedir(dp);
    free(tmp);

    return ret;
}

char* concat_file_path(int mode, char* path, char* file)
{
    char*   buf = NULL;

    switch (mode) {
        case    1:
            if ((buf = (char*)malloc(
                            sizeof(char) * strlen(file)
            )) != NULL) {
                strcpy(buf, file);
            }
            break;
        case    2:
            buf = strlion(3, path, "/", file);
            break;
        case    3:
            buf = strlion(4, path, "/", file, ".cow");
            break;
    }
    if (buf == NULL)
        fprintf(stderr, "%s: concat_file_path() failure\n",
                PROGNAME);

    return buf;
}

int check_file_stat(char* path)
{
    struct  stat st;

    if (stat(path, &st) != 0) {
        fprintf(stderr, "%s: %s: stat failure\n",
                PROGNAME, path);

        return 1;
    }

    if (S_ISDIR(st.st_mode & S_IFMT)) {
        fprintf(stderr, "%s: %s: is a directory\n",
                PROGNAME, path);
        
        return 2;
    }

    /* checking file permission */
    if (access(path, R_OK) != 0) {
        fprintf(stderr, "%s: %s: permission denied\n",
                PROGNAME, path);

        return 3;
    }

    return 0;
}

int print_string(int lines, char** str)
{
    int i   = 0,
        j   = 0,
        len = strmax(lines, str);   /* get max length */

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
    int     i       = 0,
            j       = 0;
    char*   though  = NULL;
    bool    block   = false;

    /* eyes table */
    struct  reptarg eyes[] = {
        {clsay->eflag,  EYES,   clsay->earg},
        {clsay->bflag,  EYES,   BORG_EYES},
        {clsay->dflag,  EYES,   DEAD_EYES},
        {clsay->gflag,  EYES,   GREEDY_EYES},
        {clsay->pflag,  EYES,   PARANOID_EYES},
        {clsay->sflag,  EYES,   STONED_EYES},
        {clsay->tflag,  EYES,   TIRED_EYES},
        {clsay->wflag,  EYES,   WIRED_EYES},
        {clsay->yflag,  EYES,   YOUTHFUL_EYES},
        {false,         NULL,   NULL},
    };
    /* tongue table */
    struct  reptarg tongue[] = {
        {clsay->Tflag,  TONGUE, clsay->Targ},
        {clsay->dflag,  TONGUE, DEAD_TONGUE},
        {clsay->sflag,  TONGUE, DEAD_TONGUE},
        {false,         NULL,   NULL},
    };

    /* setting thoughts */
    if (clsay->syflag == false && clsay->thflag == false) {
        though = SAY_THOUGHTS;      /* default */
    } else if (clsay->syflag == true){
        though = SAY_THOUGHTS;      /* --say switch */
    } else if (clsay->thflag == true) {
        though = THINK_THOUGHTS;    /* --think switch */
    }

    /* print cow */
    for (i = 0; i < lines; i++) {
        /* replace thoughts */
        strrep(str[i], THOUGHTS, though);

        while (strrep(str[i], "\\\\", "\\") == 0);

        /* replace eyes*/
        for (j = 0; eyes[j].haystack != NULL || eyes[j].needle != NULL; j++) {
            if (eyes[j].flag == true) {
                strrep(str[i], eyes[j].haystack, eyes[j].needle);
            }
        }
        strrep(str[i], EYES, DEFAULT_EYES);     /* default eyes*/

        /* replace tongue */
        for (j = 0; tongue[j].haystack != NULL || tongue[j].needle != NULL; j++) {
            if (tongue[j].flag == true) {
                strrep(str[i], tongue[j].haystack, tongue[j].needle);
            }
        }
        strrep(str[i], TONGUE, DEFAULT_TONGUE); /* default tongue */

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
    int     namlen,
        *   lp;
    char    dotcow[] = {".cow"};

    if ((namlen = strlen(dir->d_name)) < 4) {

        return 0;
    }
    namlen -= 4;    /* offset 4 bytes from end (.cow) */

    lp = (int*)&(dir->d_name[namlen]);

    /*
     * comparison on int
     * true: .cow
     * false: other
     */
    if (*lp == *(int*)&dotcow) {

        return 1;
    }

    return 0;
}

int list_cowfiles(void)
{
    int     i       = 0,
            j       = 0,
            entry   = 0;
    char*   env     = NULL;
    env_t*  envt    = NULL;
    struct  dirent**  list;

    /* catenate file path */
    if ((env = getenv("COWPATH")) == NULL) {
        env = COWPATH;
    }
    if ((envt = split_env(env)) == NULL) {
        fprintf(stderr, "%s: split_env() failure\n",
                PROGNAME);

        exit(10);
    }

    /* get file entry and sort */
    do {
        if ((entry = scandir(envt->envs[i], &list, selects_cowfiles, alphasort)) == -1) {
            i++;
            continue;
        } else {
            if (i != 0)
                putchar('\n');

            fprintf(stdout, "%s:\n", envt->envs[i]);
        }
        for (j = 0; j < entry; j++) {
            fprintf(stdout, "%s\n", list[j]->d_name);
            free(list[j]);
        }
        free(list);
        i++;
    } while (i < envt->envc);
    release_env_t(envt);

    exit(0);
}
