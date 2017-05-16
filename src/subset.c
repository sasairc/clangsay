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
    struct  stat st;

    if (stat(path, &st) < 0) {
        fprintf(stderr, "%s: %s: %s\n",
                PROGNAME, path, strerror(ENOENT));

        return -1;
    }

    if ((st.st_mode & S_IFMT) == S_IFDIR) {
        fprintf(stderr, "%s: %s: %s\n",
                PROGNAME, path, strerror(EISDIR));

        return -2;
    }

    if ((st.st_mode & S_IREAD) == 0) {
        fprintf(stderr, "%s: %s: %s\n",
                PROGNAME, path, strerror(EACCES));

        return -3;
    }

    if ((*fp = fopen(path, "r")) == NULL) {
        fprintf(stderr, "%s: %s\n",
                PROGNAME, strerror(errno));

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
                        smalloc(sizeof(char) * strlen(file), NULL)) == NULL)
                return -1;

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
    
        return -2;
    }

    return 0;
}

void strunsecs(struct CLANGSAY_MSG_T* msg)
{
    int i   = 0,
        j   = msg->lines - 1;

    while (i <= j && i < msg->lines) {
        strunesc(*(msg->msg + i));
        strunesc(*(msg->msg + j));
        i++;
        j--;
    }

    return;
}

int read_string(clangsay_t* clsay, int argc, int optind, char** argv)
{
    if (optind < argc) {    
        if ((clsay->msg.msg = (char**)
                    smalloc(sizeof(char*) * (argc - optind), NULL)) == NULL)
            return -1;

        while (optind < argc) {
            if ((*(clsay->msg.msg + clsay->msg.lines) = (char*)
                    smalloc(sizeof(char) * (strlen(*(argv + optind)) + 1), NULL)) == NULL)
                return -2;

            memcpy(*(clsay->msg.msg + clsay->msg.lines),
                    *(argv + optind), strlen(*(argv + optind)) + 1);
            clsay->msg.lines++;
            optind++;
        }
    } else {
        if ((clsay->msg.lines =
                    p_read_file_char(&clsay->msg.msg, TH_LINES, TH_LENGTH, stdin, 1)) < 0) {
            fprintf(stderr, "%s: read_string(): p_read_file_char() failure\n",
                    PROGNAME);

            return -3;
        }
    }
    /* remove escape sequence */
    strunsecs(&clsay->msg);

    return 0;
}

int read_cowfile(clangsay_t* clsay, FILE* fp)
{
    if (fp == NULL) {
        fprintf(stdout, "%s: read_cowfile(): fp is NULL\n",
                PROGNAME);

        return -1;
    }
    if ((clsay->cow.lines =
                p_read_file_char(&clsay->cow.cow, TH_LINES, TH_LENGTH, fp, 1)) < 0) {
        fprintf(stderr, "%s: read_cowfile(): p_read_file_char() failure\n",
                PROGNAME);

        return -2;
    }
    fclose(fp);

    return 0;
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
    maxlen = strmax_with_regex(clsay->msg.lines, clsay->msg.msg, &reg);

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
                *(clsay->msg.msg));

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
        len = mbstrlen_with_regex(*(clsay->msg.msg + i), &reg);
        if (i == 0)
            fprintf(stdout, "\n/ %s",
                    *(clsay->msg.msg + i));
        else if (i == (clsay->msg.lines - 1))
            fprintf(stdout, "\\ %s",
                    *(clsay->msg.msg + i));
        else
            fprintf(stdout, "| %s",
                    *(clsay->msg.msg + i));

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
    i = 0;
    while (i < clsay->cow.lines) {
        /* replace thoughts */
        strrep(*(clsay->cow.cow + i), THOUGHTS, though);

        while (strrep(*(clsay->cow.cow + i), "\\\\", "\\") == 0);

        /* replace eyes*/
        j = 0;
        while (eyes[j].haystack != NULL || eyes[j].needle != NULL) {
            if (eyes[j].flag == true)
                strrep(*(clsay->cow.cow + i), eyes[j].haystack, eyes[j].needle);
            j++;
        }
        /* default_eyes */
        strrep(*(clsay->cow.cow + i), EYES, DEFAULT_EYES);

        /* replace tongue */
        j = 0;
        while (tongue[j].haystack != NULL || tongue[j].needle != NULL) {
            if (tongue[j].flag == true)
                strrep(*(clsay->cow.cow + i), tongue[j].haystack, tongue[j].needle);
            j++;
        }
        /* default tongue */
        strrep(*(clsay->cow.cow + i), TONGUE, DEFAULT_TONGUE);

        /* EOC to EOC */
        if (strstr(*(clsay->cow.cow + i), "EOC")) {
            block = true;
            i++;
            continue;
        } else if (strstr(*(clsay->cow.cow + i), "EOC") && block == true) {
            block = false;
        }
        if (block == true)
            fprintf(stdout, "%s\n",
                    *(clsay->cow.cow + i));
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
