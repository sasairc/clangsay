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
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

int open_cowfile(char* path, FILE** fp)
{
    struct  stat st;

    if (stat(path, &st) < 0) {
        fprintf(stderr, "%s: %s: stat failure\n",
                PROGNAME, path);

        return -1;
    }

    if (S_ISDIR(st.st_mode & S_IFMT)) {
        fprintf(stderr, "%s: %s: is a directory\n",
                PROGNAME, path);
        
        return -2;
    }

    if ((st.st_mode & S_IREAD) == 0) {
        fprintf(stderr, "%s: %s: permission denied\n",
                PROGNAME, path);

        return -3;
    }

    if ((*fp = fopen(path, "r")) == NULL) {
        fprintf(stderr, "%s: fp is NULL\n",
                PROGNAME);

        return -4;
    }

    return 0;
}

int check_file_exists(char* path, char* file)
{
    int     ret     = 0;

    char*   tmp     = NULL;

    DIR*    dp      = NULL;

    struct  stat    st;

    struct  dirent* list;

    /* open directory */
    if ((dp = opendir(path)) == NULL)
        return 0;

    /* concat filename + .cow */
    if ((tmp = strlion(2, file, ".cow")) == NULL) {
        closedir(dp);

        return 0;
    }

    /* search cowfile from directory */
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

    /* outside of env */
    if (ret == 0) {
        if (stat(file, &st) == 0)
            ret = 1;
    }

    return ret;
}

int concat_file_path(int mode, char** dest, char* path, char* file)
{
    switch (mode) {
        case    1:
            if ((*dest = (char*)
                        malloc(sizeof(char) * strlen(file))) != NULL)
            memcpy(*dest, file, strlen(file) + 1);
            break;
        case    2:
            *dest = strlion(3, path, "/", file);
            break;
        case    3:
            *dest = strlion(4, path, "/", file, ".cow");
            break;
    }
    if (*dest == NULL) {
        fprintf(stderr, "%s: concat_file_path() failure\n",
                PROGNAME);
    
        return -1;
    }

    return 0;
}

int print_string(int msgs, char** msg)
{
    int i       = 0,
        j       = 0,
        len     = 0,
        maxlen  = strmax(msgs, msg);    /* get max length */

    /*
     * single line
     */
    putchar(' ');
    while (i <= maxlen) {
        putchar('_');
        i++;
    }
    if (msgs == 1) {
        fprintf(stdout, "\n< %s >\n ", msg[0]);

        while (maxlen >= 0) {
            putchar('-');
            maxlen--;
        }
        putchar('\n');

        return 0;
    }

    /*
     * multi line
     */
    i = 0;
    while (i < msgs) {
        j = 0;
        len = mbstrlen(msg[i]);

        if (i == 0)
            fprintf(stdout, "\n/ %s", msg[i]);
        else if (i == (msgs -1))
            fprintf(stdout, "\\ %s", msg[i]);
        else
            fprintf(stdout, "| %s", msg[i]);

        while (j < (maxlen - len)) {
            putchar(' ');
            j++;
        }

        if (i == 0)
            fprintf(stdout, " \\\n");
        else if (i == (msgs - 1))
            fprintf(stdout, " /\n ");
        else
            fprintf(stdout, " |\n");

        i++;
    }

    while (maxlen >= 0) {
        putchar('-');
        maxlen--;
    }
    putchar('\n');

    return 0;
}

int print_cow(int cows, char** cow, clangsay_t* clsay)
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
    if (clsay->syflag == false && clsay->thflag == false)
        though = SAY_THOUGHTS;      /* default */
    else if (clsay->syflag == true)
        though = SAY_THOUGHTS;      /* --say switch */
    else if (clsay->thflag == true)
        though = THINK_THOUGHTS;    /* --think switch */

    /* print cow */
    for (i = 0; i < cows; i++) {
        /* replace thoughts */
        strrep(cow[i], THOUGHTS, though);

        while (strrep(cow[i], "\\\\", "\\") == 0);

        /* replace eyes*/
        for (j = 0; eyes[j].haystack != NULL || eyes[j].needle != NULL; j++) {
            if (eyes[j].flag == true)
                strrep(cow[i], eyes[j].haystack, eyes[j].needle);
        }
        strrep(cow[i], EYES, DEFAULT_EYES);     /* default eyes*/

        /* replace tongue */
        for (j = 0; tongue[j].haystack != NULL || tongue[j].needle != NULL; j++) {
            if (tongue[j].flag == true)
                strrep(cow[i], tongue[j].haystack, tongue[j].needle);
        }
        strrep(cow[i], TONGUE, DEFAULT_TONGUE); /* default tongue */

        /* EOC to EOC */
        if (strstr(cow[i], "EOC")) {
            block = true;
            continue;
        } else if (strstr(cow[i], "EOC") && block == true) {
            block = false;
        }

        if (block == true)
            fprintf(stdout, "%s\n", cow[i]);
    }

    return 0;
}

int selects_cowfiles(const struct dirent* dir)
{
    char* tcow  = ".cow\0";

    if (strlen(dir->d_name) < 5)
        return 0;

    if (memcmp(dir->d_name + (strlen(dir->d_name) - 4), tcow, 5) == 0)
        return 1;

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
    if ((env = getenv("COWPATH")) == NULL)
        env = COWPATH;

    if ((envt = split_env(env)) == NULL) {
        fprintf(stderr, "%s: split_env() failure\n",
                PROGNAME);

        exit(9);
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
        j = 0;
        while (j < entry) {
            fprintf(stdout, "%s\n", list[j]->d_name);
            free(list[j]);
            j++;
        }
        free(list);
        i++;
    } while (i < envt->envc);
    release_env_t(envt);

    exit(0);
}
