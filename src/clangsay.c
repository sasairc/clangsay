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
#include "./string.h"
#include "./file.h"
#include "./memory.h"
#include "./env.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

int main(int argc, char* argv[])
{
    int     i       = 0,
            res     = 0,
            index   = 0,    
            cows    = 0,    /* lines of cow-file */
            msgs    = 0;    /* lines of string */

    FILE*   fp      = NULL;

    char*   path    = NULL,
        *   env     = NULL,
        *   envp    = NULL,
        **  cow     = NULL, /* cowfile */
        **  msg     = NULL; /* message */

    env_t*  envt    = NULL;

    /* flag and args */
    clangsay_t  clsay = {
        false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        NULL, NULL, NULL,
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
                    &index
                    )
           ) != -1) {
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
    if (clsay.fflag == false)
        if ((clsay.farg = getenv("DEFAULT_COWFILE")) == NULL)
            clsay.farg = DEFAULT_COWFILE;

    /* check env $COWPATH */
    if ((env = getenv("COWPATH")) == NULL)
        env = COWPATH;

    /* env string to struct */
    if ((envt = split_env(env)) == NULL) {
        fprintf(stderr, "%s: split_env() failure\n",
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
        if ((res = check_file_exists(envt->envs[i], clsay.farg)) != 0) {
            envp = envt->envs[i];

            break;
        }
        i++;
    } while (i < envt->envc);

    if (res == 0) {
        fprintf(stderr, "%s: %s: cowfile not found\n",
                PROGNAME, clsay.farg);
        release(NULL, envt, NULL, 0, NULL, 0, NULL);

        return 2;
    }

    if ((path = concat_file_path(res, envp, clsay.farg)) == NULL) {
        release(NULL, envt, NULL, 0, NULL, 0, NULL);

        return 3;
    }

    if (open_cowfile(path, &fp) < 0) {
        release(NULL, envt, path, 0, NULL, 0, NULL);

        return 4;
    }

    /* 
     * reading stdin or args to array
     * true : arguments
     * false: pipe
     */ 
    if (optind < argc) {    
        /* allocate array for y coordinate (strings) */
        if ((msg = (char**)
                    malloc(sizeof(char*) * (argc - optind))) == NULL) {
            fprintf(stderr, "%s: malloc() failure\n",
                    PROGNAME);
            release(fp, envt, path, 0, NULL, 0, NULL);
            
            return 5;
        }

        i = 0;
        while (optind < argc) {
            if ((msg[i] = (char*)
                        malloc(sizeof(char) * (strlen(argv[optind]) + 1))) == NULL) {
                fprintf(stderr, "%s: malloc() failure\n",
                        PROGNAME);
                release(fp, envt, path, msgs, msg, 0, NULL);

                return 6;
            }
            memcpy(msg[i], argv[optind], strlen(argv[optind]) + 1);
            i++;
            optind++;
        }
        msgs = i;
    } else {
        if ((msgs = p_read_file_char(&msg, TH_LINES, TH_LENGTH, stdin, 1)) < 0) {
            fprintf(stderr, "%s: p_read_file_char() failure\n",
                    PROGNAME);
            release(fp, envt, path, 0, NULL, 0, NULL);

            return 7;
        }
    }

    /* remove escape sequence */
    i = 0;
    while (i < msgs) {
        strunesc(msg[i]);
        i++;
    }

    /* reading cow file to array */
    if ((cows = p_read_file_char(&cow, TH_LINES, TH_LENGTH, fp, 1)) < 0) {
        fprintf(stderr, "%s: p_read_file_char() failure\n",
                PROGNAME);
        release(fp, envt, path, msgs, msg, 0, NULL);

        return 8;
    }

    /* print string */
    print_string(msgs, msg);
    /* print cow */
    print_cow(cows, cow, &clsay);
    /* memory release */
    release(fp, envt, path, msgs, msg, cows, cow);

    return 0;
}

void release(FILE* fp, env_t* envt, char* path, int lines1, char** buf1, int lines2, char** buf2)
{
    if (fp != NULL) {
        fclose(fp);
        fp = NULL;
    }
    if (envt != NULL) {
        release_env_t(envt);
    }
    if (path != NULL) {
        free(path);
        path = NULL;
    }
    if (buf1 != NULL) {
        free2d(buf1, lines1);
    }
    if (buf2 != NULL) {
        free2d(buf2, lines2);
    }

    return;
}
