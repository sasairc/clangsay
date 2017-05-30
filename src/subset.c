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
#include "./memory.h"
#include "./clangsay.h"
#include "./subset.h"
#include "./string.h"
#include "./file.h"
#include "./env.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <regex.h>
#include <errno.h>
#include <sys/stat.h>

int open_cowfile(char* path, FILE** fp)
{
    short   status  = 0;

    struct  stat st;

    if (stat(path, &st) < 0) {
        status = -1; goto ERR;
    }

    if ((st.st_mode & S_IFMT) == S_IFDIR) {
        status = -2; goto ERR;
    }

    if ((st.st_mode & S_IREAD) == 0) {
        status = -3; goto ERR;
    }

    if ((*fp = fopen(path, "r")) == NULL) {
        status = -4; goto ERR;
    }

    return 0;

ERR:
    switch (status) {
        case    -1:
            fprintf(stderr, "%s: %s: %s\n",
                    PROGNAME, path, strerror(ENOENT));
            break;
        case    -2:
            fprintf(stderr, "%s: %s: %s\n",
                    PROGNAME, path, strerror(EISDIR));
            break;
        case    -3:
            fprintf(stderr, "%s: %s: %s\n",
                    PROGNAME, path, strerror(EACCES));
            break;
        case    -4:
            fprintf(stderr, "%s: %s\n",
                    PROGNAME, strerror(errno));
            break;
    }

    return status;
}

int check_cowfile_exists(char* env, char* file, char** dest)
{
    int     i       = 0,
            res     = 0;

    short   status  = 0;

    char*   path    = NULL,
        *   envp    = NULL;

    env_t*  envt    = NULL;

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
    short   status  = 0;

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

void strunsecs(struct CLANGSAY_MSG_T* msg)
{
    int i   = 0,
        j   = msg->lines - 1;

    while (i <= j && i < msg->lines) {
        strunesc(*(msg->data + i));
        strunesc(*(msg->data + j));
        i++;
        j--;
    }

    return;
}

int read_string(clangsay_t* clsay, int argc, int optind, char** argv)
{
    short   status  = 0;

    if (optind < argc) {    
        if ((clsay->msg.data = (char**)
                    smalloc(sizeof(char*) * (argc - optind), NULL)) == NULL) {
            status = -1; goto ERR;
        }
        while (optind < argc) {
            if ((*(clsay->msg.data + clsay->msg.lines) = (char*)
                    smalloc(sizeof(char) * (strlen(*(argv + optind)) + 1), NULL)) == NULL) {
                status = -2; goto ERR;
            }
            memcpy(*(clsay->msg.data + clsay->msg.lines),
                    *(argv + optind), strlen(*(argv + optind)) + 1);
            clsay->msg.lines++;
            optind++;
        }
    } else {
        if ((clsay->msg.lines =
                    p_read_file_char(&clsay->msg.data, TH_LINES, TH_LENGTH, stdin, 1)) < 0) {
            status = -3; goto ERR;
        }
    }
    /* remove escape sequence */
    strunsecs(&clsay->msg);

    return 0;

ERR:
    switch (status) {
        case    -1:
        case    -2:
            break;
        case    -3:
            fprintf(stderr, "%s: read_string(): p_read_file_char() failure\n",
                    PROGNAME);
            break;
    }

    return status;
}

int read_cowfile(clangsay_t* clsay, FILE* fp)
{
    short   status  = 0;

    if (fp == NULL) {
        status = -1; goto ERR;
    }
    if ((clsay->cow.lines =
                p_read_file_char(&clsay->cow.data, TH_LINES, TH_LENGTH, fp, 1)) < 0) {
        status = -2; goto ERR;
    }
    fclose(fp);

    return 0;

ERR:
    switch (status) {
        case    -1:
            fprintf(stdout, "%s: read_cowfile(): fp is NULL\n",
                    PROGNAME);
            break;
        case    -2:
            fprintf(stderr, "%s: read_cowfile(): p_read_file_char() failure\n",
                    PROGNAME);
        default:
            if (fp != NULL) {
                fclose(fp);
                fp = NULL;
            }
    }

    return status;
}

int print_string(clangsay_t* clsay)
{
    int         i       = 0,
                j       = 0,
                len     = 0,
                maxlen  = 0;    /* get max length */

    regex_t     reg;

    /* compile regex */
    regcomp(&reg, ANSI_ESCSEQ, REG_EXTENDED);

    /* get max length */
    maxlen = strmax_with_regex(clsay->msg.lines, clsay->msg.data, &reg);

    /*
     * single line
     */
    putchar(' ');
    i = maxlen;
    while (i >= 0) {
        putchar('_');
        i--;
    }
    if (clsay->msg.lines == 1) {
        fprintf(stdout, "\n< %s >\n ",
                *(clsay->msg.data));

        while (maxlen >= 0) {
            putchar('-');
            maxlen--;
        }
        puts("");

        return 0;
    }

    /*
     * multi line
     */
    i = 0;
    while (i < clsay->msg.lines) {
        len = mbstrlen_with_regex(*(clsay->msg.data + i), &reg);
        if (i == 0)
            fprintf(stdout, "\n/ %s",
                    *(clsay->msg.data + i));
        else if (i == (clsay->msg.lines - 1))
            fprintf(stdout, "\\ %s",
                    *(clsay->msg.data + i));
        else
            fprintf(stdout, "| %s",
                    *(clsay->msg.data + i));

        j = maxlen - len;
        while (j > 0) {
            putchar(' ');
            j--;
        }

        if (i == 0)
            puts(" \\");
        else if (i == (clsay->msg.lines - 1))
            fprintf(stdout, " /\n ");
        else
            puts(" |");

        i++;
    }
    while (maxlen >= 0) {
        putchar('-');
        maxlen--;
    }
    puts("");

    regfree(&reg);

    return 0;
}

int print_cow(clangsay_t* clsay)
{
    int     i       = 0,
            j       = 0;

    char*   though  = NULL;

    short   block   = 0;

    /* eyes table */
    struct  reptarg eyes[] = {
        {MODE_M_EYE,    EYES,   clsay->eye},
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
        {MODE_M_TONGUE, TONGUE, clsay->tongue},
        {MODE_DEAD,     TONGUE, DEAD_TONGUE},
        {MODE_STONED,   TONGUE, DEAD_TONGUE},
        {0,             NULL,   NULL},
    };

    /* setting thoughts
     * 1. default
     * 2. --say
     * 3. --think
     */
    if (!(clsay->mode & MODE_SAY) && !(clsay->mode & MODE_THINK))
        though = SAY_THOUGHTS;
    else if (clsay->mode & MODE_SAY)
        though = SAY_THOUGHTS;
    else if (clsay->mode & MODE_THINK)
        though = THINK_THOUGHTS;

    /* print cow */
    i = 0;
    while (i < clsay->cow.lines) {
        /* replace thoughts */
        strrep(*(clsay->cow.data + i), THOUGHTS, though);

        while (strrep(*(clsay->cow.data + i), "\\\\", "\\") == 0);

        /* replace eyes*/
        j = 0;
        while (eyes[j].haystack != NULL || eyes[j].needle != NULL) {
            if (clsay->mode & eyes[j].mode)
                strrep(*(clsay->cow.data + i), eyes[j].haystack, eyes[j].needle);
            j++;
        }
        /* default_eyes */
        strrep(*(clsay->cow.data + i), EYES, DEFAULT_EYES);

        /* replace tongue */
        j = 0;
        while (tongue[j].haystack != NULL || tongue[j].needle != NULL) {
            if (clsay->mode & tongue[j].mode)
                strrep(*(clsay->cow.data + i), tongue[j].haystack, tongue[j].needle);
            j++;
        }
        /* default tongue */
        strrep(*(clsay->cow.data + i), TONGUE, DEFAULT_TONGUE);

        /* EOC to EOC */
        if (strstr(*(clsay->cow.data + i), "EOC")) {
            block = 1;
            i++;
            continue;
        } else if (strstr(*(clsay->cow.data + i), "EOC") && block == 1) {
            block = 0;
        }
        if (block == 1)
            fprintf(stdout, "%s\n",
                    *(clsay->cow.data + i));
        i++;
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

    if (split_env(env, &envt) < 0) {
        fprintf(stderr, "%s: list_cowfiles(): split_env() failure\n",
                PROGNAME);

        exit(7);
    }

    /* get file entry and sort */
    do {
        if ((entry = scandir(*(envt->envs + i),
                        &list, selects_cowfiles, alphasort)) == -1) {
            i++;
            continue;
        } else {
            if (i != 0)
                puts("");

            fprintf(stdout, "%s:\n",
                    *(envt->envs + i));
        }
        j = 0;
        while (j < entry) {
            fprintf(stdout, "%s\n",
                    list[j]->d_name);
            free(*(list + j));
            j++;
        }
        free(list);
        i++;
    } while (i < envt->envc);
    release_env_t(envt);

    exit(0);
}
