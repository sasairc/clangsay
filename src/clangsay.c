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
#include "./cow.h"
#include "./msg.h"
#include "./info.h"
#include "./list.h"
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

static void release(COW* cow, MSG* msg);

int main(int argc, char* argv[])
{
    int     res     = 0,
            index   = 0,
            status  = 0;

    char*   cowfile = NULL;

    COW*    cow     = NULL;

    MSG*    msg     = NULL;

    COWOPT  opt     = {
        COWOPT_ALLNO_FLAGS,
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
                opt.mode |= MODE_M_EYE;
                opt.eye= optarg;
                break;
            case    'T':
                opt.mode |= MODE_M_TONGUE;
                opt.tongue = optarg;
                break;
            case    'f':
                cowfile = optarg;
                break;
            case    'b':
                opt.mode |= MODE_BORG;
                break;
            case    'd':
                opt.mode |= MODE_DEAD;
                break;
            case    'g':
                opt.mode |= MODE_GREEDY;
                break;
            case    'p':
                opt.mode |= MODE_PARANOID;
                break;
            case    's':
                opt.mode |= MODE_STONED;
                break;
            case    't':
                opt.mode |= MODE_TIRED;
                break;
            case    'w':
                opt.mode |= MODE_WIRED;
                break;
            case    'y':
                opt.mode |= MODE_YOUTHFUL;
                break;
            case    'l':
                list_cowfiles();
            case    0:
                opt.mode |= MODE_SAY;
                break;
            case    1:
                opt.mode |= MODE_THINK;
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
    if (cowfile == NULL) {
        if ((cowfile = getenv("DEFAULT_COWFILE")) == NULL)
            cowfile = DEFAULT_COWFILE;
    }

    /* initialize cow */
    if (init_cow(&cow) < 0) {
        status = 1; goto ERR;
    }

    /* open cow file */
    if (cow->open(&cow, cowfile) < 0) {
        status = 2; goto ERR;
    }

    /* reading cow file to array */
    if (cow->read(&cow) < 0) {
        status = 3; goto ERR;
    }

    /* initialize msg */
    if (init_msg(&msg) < 0) {
        status = 4; goto ERR;
    }

    /* read argv or stdin */
    if (msg->read(&msg, argc, optind, argv) < 0) {
        status = 5; goto ERR;
    }

    /* print string */
    if (msg->print(msg) < 0) {
        status = 6; goto ERR;
    }

    /* print cow */
    cow->print(cow, &opt);

    /* release memory */
    release(cow, msg);

    return 0;

ERR:
    release(cow, msg);

    return status;
}

static
void release(COW* cow, MSG* msg)
{
    if (cow != NULL)
        cow->release(cow);

    if (msg != NULL)
        msg->release(msg);

    return;
}
