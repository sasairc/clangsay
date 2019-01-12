/*
 * clangsay -  The classic cowsay program, written in C.
 *
 * cow.c
 *
 * Copyright (c) 2015 sasairc
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar.HocevarHocevar See the COPYING file or http://www.wtfpl.net/
 * for more details.
 */

#include "./config.h"
#include "./cow.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>

#ifdef  WITH_SHARED
#include <benly/memory.h>
#include <benly/string.h>
#include <benly/file.h>
#include <benly/env.h>
#else
#include <libbenly/src/memory.h>
#include <libbenly/src/string.h>
#include <libbenly/src/file.h>
#include <libbenly/src/env.h>
/* WITH_SHARED */
#endif

struct  reptarg {
    int     mode;
    char*   haystack;
    char*   needle;
};

static int open_cow(COW** cow, char* file);
static int read_cow(COW** cow);
static int print_cow(COW* cow, COWOPT* opt);
static void release_cow(COW* cow);

static void replace_escaped_backslash(char* str);
static int check_cowfile_exists(char* file, char** dest);
static int check_file_exists(char* path, char* file);
static int concat_file_path(int mode, char** dest, char* path, char* file);

int init_cow(COW** cow)
{
    int     status  = 0;

    COW*    tmp     = NULL;

    if ((tmp = (COW*)
                malloc(sizeof(COW))) == NULL) {
        status = -1; goto ERR;
    }
    do {
        tmp->fp         = NULL;
        tmp->data       = NULL;
        tmp->lines      = 0;
        tmp->open       = open_cow;
        tmp->read       = read_cow;
        tmp->print      = print_cow;
        tmp->release    = release_cow;
    } while (0);
    *cow = tmp;

    return 0;

ERR:
    switch (status) {
        case    -1:
            fprintf(stderr, "%s: malloc(): %s\n",
                    PROGNAME, strerror(errno));
            break;
    }

    return status;
}

static
int open_cow(COW** cow, char* file)
{
    char*   path    = NULL;

    int     status  = 0;

    struct  stat st;

    if (check_cowfile_exists(file, &path) < 0) {
        status = -1; goto ERR;
    }

    if (stat(path, &st) < 0) {
        status = -2; goto ERR;
    }

    if ((st.st_mode & S_IFMT) == S_IFDIR) {
        status = -3; goto ERR;
    }

    if ((st.st_mode & S_IREAD) == 0) {
        status = -4; goto ERR;
    }

    if (((*cow)->fp = fopen(path, "r")) == NULL) {
        status = -5; goto ERR;
    }

    if (path != NULL) {
        free(path);
        path = NULL;
    }

    return 0;

ERR:
    switch (status) {
        case    -1:
            break;
        case    -2:
            fprintf(stderr, "%s: %s: %s\n",
                    PROGNAME, path, strerror(ENOENT));
            break;
        case    -3:
            fprintf(stderr, "%s: %s: %s\n",
                    PROGNAME, path, strerror(EISDIR));
            break;
        case    -4:
            fprintf(stderr, "%s: %s: %s\n",
                    PROGNAME, path, strerror(EACCES));
            break;
        case    -5:
            fprintf(stderr, "%s: %s\n",
                    PROGNAME, strerror(errno));
            break;
    }
    if (path != NULL) {
        free(path);
        path = NULL;
    }

    return status;
}

static
int read_cow(COW** cow)
{
    int     status  = 0;

    if ((*cow)->fp == NULL) {
        status = -1; goto ERR;
    }
    if (((*cow)->lines =
                load_file_to_array(&(*cow)->data, TH_LINES, TH_LENGTH, (*cow)->fp)) < 0) {
        status = -2; goto ERR;
    }
    fclose((*cow)->fp);

    return 0;

ERR:
    switch (status) {
        case    -1:
            fprintf(stdout, "%s: read_cowfile(): fp is NULL\n",
                    PROGNAME);
            break;
        case    -2:
            fprintf(stderr, "%s: read_cowfile(): load_file_to_array(): %s\n",
                    PROGNAME, strerror(errno));
        default:
            if ((*cow)->fp != NULL) {
                fclose((*cow)->fp);
                (*cow)->fp = NULL;
            }
    }

    return status;
}

static
int print_cow(COW* cow, COWOPT* opt)
{
    int     i       = 0,
            j       = 0;

    char*   though  = NULL;

    short   block   = 0;

    /* eyes table */
    struct  reptarg eyes[] = {
        {MODE_M_EYE,    EYES,   opt->eye},
        {MODE_BORG,     EYES,   BORG_EYES},
        {MODE_DEAD,     EYES,   DEAD_EYES},
        {MODE_GREEDY,   EYES,   GREEDY_EYES},
        {MODE_PARANOID, EYES,   PARANOID_EYES},
        {MODE_STONED,   EYES,   STONED_EYES},
        {MODE_TIRED,    EYES,   TIRED_EYES},
        {MODE_WIRED,    EYES,   WIRED_EYES},
        {MODE_YOUTHFUL, EYES,   YOUTHFUL_EYES},
        {0,             NULL,   NULL},
    };
    /* tongue table */
    struct  reptarg tongue[] = {
        {MODE_M_TONGUE, TONGUE, opt->tongue},
        {MODE_DEAD,     TONGUE, DEAD_TONGUE},
        {MODE_STONED,   TONGUE, DEAD_TONGUE},
        {0,             NULL,   NULL},
    };

    /* setting thoughts
     * 1. default
     * 2. --say
     * 3. --think
     */
    if (!(opt->mode & MODE_SAY) && !(opt->mode & MODE_THINK))
        though = SAY_THOUGHTS;
    else if (opt->mode & MODE_SAY)
        though = SAY_THOUGHTS;
    else if (opt->mode & MODE_THINK)
        though = THINK_THOUGHTS;

    /* print cow */
    i = 0;
    while (i < cow->lines) {
        /* replace thoughts */
        strrep(*(cow->data + i), THOUGHTS, though);
        /* replace backslash ( \\ -> \ ) */
        replace_escaped_backslash(*(cow->data + i));
        /* replace eyes*/
        j = 0;
        while (eyes[j].haystack != NULL || eyes[j].needle != NULL) {
            if (opt->mode & eyes[j].mode)
                strrep(*(cow->data + i), eyes[j].haystack, eyes[j].needle);
            j++;
        }
        /* default_eyes */
        strrep(*(cow->data + i), EYES, DEFAULT_EYES);

        /* replace tongue */
        j = 0;
        while (tongue[j].haystack != NULL || tongue[j].needle != NULL) {
            if (opt->mode & tongue[j].mode)
                strrep(*(cow->data + i), tongue[j].haystack, tongue[j].needle);
            j++;
        }
        /* default tongue */
        strrep(*(cow->data + i), TONGUE, DEFAULT_TONGUE);

        /* EOC to EOC */
        if (strstr(*(cow->data + i), "EOC")) {
            block = 1;
            i++;
            continue;
        } else if (strstr(*(cow->data + i), "EOC") && block == 1) {
            block = 0;
        }
        if (block == 1)
            fprintf(stdout, "%s\n",
                    *(cow->data + i));
        i++;
    }

    return 0;
}

static
void release_cow(COW* cow)
{
    int     i   = 0,
            j   = 0;

    if (cow != NULL) {
        if (cow->data != NULL) {
            i = 0;
            j = cow->lines - 1;
            while (i <= j) {
                if (*(cow->data + i) != NULL) {
                    free(*(cow->data + i));
                    *(cow->data + i) = NULL;
                }
                if (*(cow->data + j) != NULL) {
                    free(*(cow->data + j));
                    *(cow->data + j) = NULL;
                }
                i++;
                j--;
            }
            free(cow->data);
            cow->data = NULL;
        }
        free(cow);
        cow = NULL;
    }

    return;
}

static
void replace_escaped_backslash(char* str)
{
    int     c       = 0;

    char*   p       = str;

    while (*p != '\0') {
        if ((*p == '\\' && *(p + 1) != '\0') && *(p + 1) == '\\') {
            c++;
            p++;
        }
        p++;
    }
    while (c) {
        strrep(str, "\\\\", "\\");
        c--;
    }

    return;
}

static
int check_cowfile_exists(char* file, char** dest)
{
    int     i       = 0,
            res     = 0,
            status  = 0;

    char*   env     = NULL,
        *   path    = NULL,
        *   envp    = NULL;

    env_t*  envt    = NULL;

    /* check env $COWPATH */
    if ((env = getenv("COWPATH")) == NULL)
        env = COWPATH;

    /* env string to struct */
    if (split_env(env, &envt) < 0) {
        status = -1; goto ERR;
    }

    /*
     * # check_file_exists()
     *
     * 0: not found
     * 1: keep argument(path)
     * 2: long argument(filename.cow)
     * 3: short argument(filename)
     */
    do {
        if ((res = check_file_exists(*(envt->envs + i), file)) != 0) {
            envp = *(envt->envs + i);

            break;
        }
        i++;
    } while (i < envt->envc);

    /* cow file not found */
    if (res == 0) {
        status = -2; goto ERR;
    }

    /* concat path */
    if (concat_file_path(res, &path, envp, file) < 0) {
        status = -3; goto ERR;
    }

    release_env_t(envt);
    *dest = path;

    return 0;

ERR:
    switch (status) {
        case    -1:
            fprintf(stderr, "%s: check_file_exists(): split_env() failure\n",
                    PROGNAME);
            break;
        case    -2:
            fprintf(stderr, "%s: %s: cowfile not found\n",
                    PROGNAME, file);
        case    -3:
        default:
            if (envt != NULL)
                release_env_t(envt);

            if (path != NULL)
                free(path);
    }

    return status;
}

static
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

static
int concat_file_path(int mode, char** dest, char* path, char* file)
{
    int     status  = 0;

    switch (mode) {
        case    1:
            if ((*dest = (char*)
                        smalloc(sizeof(char) * strlen(file), NULL)) == NULL) {
                status = -1; goto ERR;
            }
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
        status =  -2; goto ERR;
    }

    return 0;

ERR:
    switch (status) {
        case    -1:
            break;
        case    -2:
            fprintf(stderr, "%s: concat_file_path() failure\n",
                    PROGNAME);
            break;
    }

    return status;
}
