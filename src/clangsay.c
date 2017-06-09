/*
 * clangsay -  The classic cowsay program, written in C.
 *
 * clangsay.c
 *
 * Copyright (c) 2015 sasairc
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar.HocevarHocevar See the COPYING file or http://www.wtfpl.net/
 * for more details.
 */

#include "./config.h"
#include "./clangsay.h"
#include "./info.h"
#include "./subset.h"
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

int main(int argc, char* argv[])
{
    int     res     = 0,
            index   = 0;

    FILE*   fp      = NULL;

    char*   path    = NULL,
        *   env     = NULL;

    /* flag and args */
    clangsay_t  clsay = {
        CLANGSAY_ALLNO_FLAG,
    };

    /* option for getopt_long() */
    struct  option opts[] = {
        {"eye",      required_argument, NULL, 'e'},
        {"tongue",   required_argument, NULL, 'T'},
        {"file",     required_argument, NULL, 'f'},
        {"nowrap",   no_argument,       NULL, 'n'},
        {"borg",     no_argument,       NULL, 'b'},
        {"dead",     no_argument,       NULL, 'd'},
        {"greedy",   no_argument,       NULL, 'g'},
        {"paranoid", no_argument,       NULL, 'p'},
        {"stoned",   no_argument,       NULL, 's'},
        {"tired",    no_argument,       NULL, 't'},
        {"wired",    no_argument,       NULL, 'w'},
        {"youtuful", no_argument,       NULL, 'y'},
        {"list",     no_argument,       NULL, 'l'},
        {"say",      no_argument,       NULL,  0 },
        {"think",    no_argument,       NULL,  1 },
        {"help",     no_argument,       NULL,  2 },
        {"version",  no_argument,       NULL,  3 },
        {0, 0, 0, 0},
    };

    /* processing of arguments */
    while ((res = getopt_long(
                    argc,
                    argv,
                    "nW;bdgpstwye:T:f:l",
                    opts,
                    &index)) != -1) {
        switch (res) {
            case    'e':
                clsay.mode |= MODE_M_EYE;
                clsay.eye= optarg;
                break;
            case    'T':
                clsay.mode |= MODE_M_TONGUE;
                clsay.tongue = optarg;
                break;
            case    'f':
                clsay.mode |= MODE_M_FILE;
                clsay.file = optarg;
                break;
            case    'b':
                clsay.mode |= MODE_BORG;
                break;
            case    'd':
                clsay.mode |= MODE_DEAD;
                break;
            case    'g':
                clsay.mode |= MODE_GREEDY;
                break;
            case    'p':
                clsay.mode |= MODE_PARANOID;
                break;
            case    's':
                clsay.mode |= MODE_STONED;
                break;
            case    't':
                clsay.mode |= MODE_TIRED;
                break;
            case    'w':
                clsay.mode |= MODE_WIRED;
                break;
            case    'y':
                clsay.mode |= MODE_YOUTHFUL;
                break;
            case    'l':
                list_cowfiles();
            case    0:
                clsay.mode |= MODE_SAY;
                break;
            case    1:
                clsay.mode |= MODE_THINK;
                break;
            case    2:
                print_usage();
            case    3:
                print_version();
            case    '?':
                return -1;
        }
    }

    /* check env $DEFAULT_COWFILE */
    if (!(clsay.mode & MODE_M_FILE)) {
        if ((clsay.file = getenv("DEFAULT_COWFILE")) == NULL)
            clsay.file = DEFAULT_COWFILE;
    }

    /* check env $COWPATH */
    if ((env = getenv("COWPATH")) == NULL)
        env = COWPATH;

    /* check cow file exists */
    if (check_cowfile_exists(env, clsay.file, &path) < 0)
        return 1;

    /* open cow file */
    if (open_cowfile(path, &fp) < 0) {
        release(path, NULL);

        return 2;
    }

    /* reading cow file to array */
    if (read_cowfile(&clsay, fp) < 0) {
        release(path, &clsay);

        return 3;
    }

    /* read argv or stdin */
    if (read_string(&clsay, argc, optind, argv) < 0) {
        release(path, &clsay);

        return 4;
    }

    /* print string */
    print_string(&clsay);
    /* print cow */
    print_cow(&clsay);
    /* memory release */
    release(path, &clsay);

    return 0;
}

void release(char* path, clangsay_t* clsay)
{
    int     i   = 0,
            j   = 0;

    if (path != NULL) {
        free(path);
        path = NULL;
    }
    if (clsay != NULL) {
        if (clsay->cow.data != NULL) {
            i = 0;
            j = clsay->cow.lines - 1;
            while (i <= j) {
                if (*(clsay->cow.data + i) != NULL) {
                    free(*(clsay->cow.data + i));
                    *(clsay->cow.data + i) = NULL;
                }
                if (*(clsay->cow.data + j) != NULL) {
                    free(*(clsay->cow.data + j));
                    *(clsay->cow.data + j) = NULL;
                }
                i++;
                j--;
            }
            free(clsay->cow.data);
            clsay->cow.data = NULL;
        }
        if (clsay->msg.data != NULL) {
            i = 0;
            j = clsay->msg.lines - 1;
            while (i <= j) {
                if (*(clsay->msg.data + i) != NULL) {
                    free(*(clsay->msg.data + i));
                    *(clsay->msg.data + i) = NULL;
                }
                if (*(clsay->msg.data + j) != NULL) {
                    free(*(clsay->msg.data + j));
                    *(clsay->msg.data + j) = NULL;
                }
                i++;
                j--;
            }
            free(clsay->msg.data);
            clsay->msg.data = NULL;
        }
    }

    return;
}
