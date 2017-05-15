/*
 * clangsay -  The classic cowsay program, written in C.

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
#include "./string.h"
#include "./file.h"
#include "./env.h"
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

int main(int argc, char* argv[])
{
    int     i       = 0,
            res     = 0,
            index   = 0;

    FILE*   fp      = NULL;

    char*   path    = NULL,
        *   env     = NULL,
        *   envp    = NULL;

    env_t*  envt    = NULL;

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
                clsay.earg = optarg;
                clsay.eflag = true;
                break;
            case    'T':
                clsay.Targ = optarg;
                clsay.Tflag = true;
                break;
            case    'f':
                clsay.farg = optarg;
                clsay.fflag = true;
                break;
            case    'b':
                clsay.bflag = true;
                break;
            case    'd':
                clsay.dflag = true;
                break;
            case    'g':
                clsay.gflag = true;
                break;
            case    'p':
                clsay.pflag = true;
                break;
            case    's':
                clsay.sflag = true;
                break;
            case    't':
                clsay.tflag = true;
                break;
            case    'w':
                clsay.wflag = true;
                break;
            case    'y':
                clsay.yflag = true;
                break;
            case    'l':
                list_cowfiles();
            case    0:
                clsay.syflag = true;
                break;
            case    1:
                clsay.thflag = true;
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
    if (clsay.fflag == false) {
        if ((clsay.farg = getenv("DEFAULT_COWFILE")) == NULL)
            clsay.farg = DEFAULT_COWFILE;
    }

    /* check env $COWPATH */
    if ((env = getenv("COWPATH")) == NULL)
        env = COWPATH;

    /* env string to struct */
    if (split_env(env, &envt) < 0) {
        fprintf(stderr, "%s: main(): split_env() failure\n",
                PROGNAME);

        return 1;
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
        if ((res = check_file_exists(*(envt->envs + i), clsay.farg)) != 0) {
            envp = *(envt->envs + i);

            break;
        }
        i++;
    } while (i < envt->envc);

    if (res == 0) {
        fprintf(stderr, "%s: %s: cowfile not found\n",
                PROGNAME, clsay.farg);
        release(envt, NULL, NULL);

        return 2;
    }

    if (concat_file_path(res, &path, envp, clsay.farg) < 0) {
        release(envt, NULL, NULL);

        return 3;
    }

    if (open_cowfile(path, &fp) < 0) {
        release(envt, path, NULL);

        return 4;
    }

    /* reading cow file to array */
    if (read_cowfile(&clsay, fp) < 0) {
        release(envt, path, &clsay);

        return 5;
    }

    /* read argv or stdin */
    if (read_string(&clsay, argc, optind, argv) < 0) {
        release(envt, path, &clsay);

        return 6;
    }

    /* print string */
    print_string(&clsay);
    /* print cow */
    print_cow(&clsay);
    /* memory release */
    release(envt, path, &clsay);

    return 0;
}

void release(env_t* envt, char* path, clangsay_t* clsay)
{
    int     i   = 0,
            j   = 0;

    if (envt != NULL) {
        release_env_t(envt);
    }
    if (path != NULL) {
        free(path);
        path = NULL;
    }
    if (clsay->cow.cow != NULL) {
        i = 0;
        j = clsay->cow.lines - 1;
        while (i <= j) {
            if (*(clsay->cow.cow + i) != NULL) {
                free(*(clsay->cow.cow + i));
                *(clsay->cow.cow + i) = NULL;
            }
            if (*(clsay->cow.cow + j) != NULL) {
                free(*(clsay->cow.cow + j));
                *(clsay->cow.cow + j) = NULL;
            }
            i++;
            j--;
        }
        free(clsay->cow.cow);
        clsay->cow.cow = NULL;
    }
    if (clsay->msg.msg != NULL) {
        i = 0;
        j = clsay->msg.lines - 1;
        while (i <= j) {
            if (*(clsay->msg.msg + i) != NULL) {
                free(*(clsay->msg.msg + i));
                *(clsay->msg.msg + i) = NULL;
            }
            if (*(clsay->msg.msg + j) != NULL) {
                free(*(clsay->msg.msg + j));
                *(clsay->msg.msg + j) = NULL;
            }
            i++;
            j--;
        }
        free(clsay->msg.msg);
        clsay->msg.msg = NULL;
    }

    return;
}
