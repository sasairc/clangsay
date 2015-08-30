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
            res     = 0,    /* use getopt_long() */
            index   = 0,    
            lines   = 0,    /* lines of cowfile*/
            stdins  = 0;    /* lines of string */
    FILE*   fp      = NULL; /* cow-file */
    char*   path    = NULL, /* .cow file */
        *   env     = NULL, /* string of $COWPATH */
        *   envp    = NULL,
        **  cowbuf  = NULL, /* string buffer (cow) */
        **  strbuf  = NULL; /* string buffer (string) */
    env_t*  envt    = NULL; /* environment variable */

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

    if (clsay.fflag == false)
        clsay.farg = DEFAULT_COWFILE;

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
     * 0: error
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

    switch (res) {
        case    0:
            fprintf(stderr, "%s: %s: cowfile not found\n",
                    PROGNAME, clsay.farg);
            release(NULL, envt, NULL, 0, NULL, 0, NULL);

            return 2;
        case    1:
            if ((path = (char*)malloc(
                            sizeof(char) * strlen(clsay.farg)
                            )) != NULL) {
                strcpy(path, clsay.farg);
            }
            break;
        case    2:
            path = strlion(3, envp, "/", clsay.farg);
            break;
        case    3:
            path = strlion(4, envp, "/", clsay.farg, ".cow");
            break;
    }

    if (path == NULL) {
        fprintf(stderr, "%s: strlion() failure\n",
                PROGNAME);
        release(NULL, envt, NULL, 0, NULL, 0, NULL);

        return 3;
    }
    if (check_file_stat(path) != 0) {
        release(NULL, envt, path, 0, NULL, 0, NULL);

        return 4;
    }
    if ((fp = open_file(path)) == NULL) {
        release(NULL, envt, path, 0, NULL, 0, NULL);

        return 5;
    }

    /* 
     * reading stdin or args to array
     * true : arguments
     * false: pipe
     */ 
    if (optind < argc) {    
        strbuf = (char**)malloc(sizeof(char*) * (argc - optind));   /* allocate array for y coordinate (strings) */
        if (strbuf == NULL) {
            fprintf(stderr, "%s: malloc() failure\n",
                    PROGNAME);
            release(fp, envt, path, 0, NULL, 0, NULL);
            
            return 6;
        }
        for (i = 0; optind < argc; optind++, i++) {
            strbuf[i] = (char*)malloc(sizeof(char) * (strlen(argv[optind]) + 1));
            if (strbuf[i] == NULL) {
                fprintf(stderr, "%s: malloc() failure\n",
                        PROGNAME);
                release(fp, envt, path, stdins, strbuf, 0, NULL);

                return 7;
            }
            strcpy(strbuf[i], argv[optind]);
        }
        stdins = i;
    } else {
        if ((strbuf = p_read_file_char(TH_LINES, TH_LENGTH, stdin)) == NULL) {
            fprintf(stderr, "%s: p_read_file_char() failure\n",
                    PROGNAME);
            release(fp, envt, path, 0, NULL, 0, NULL);

            return 8;
        }
        stdins = p_count_file_lines(strbuf);    /* count file lines */
    }

    /* reading cow file to array */
    if ((cowbuf = p_read_file_char(TH_LINES, TH_LENGTH, fp)) == NULL) {
        fprintf(stderr, "%s: p_read_file_char() failure\n",
                PROGNAME);
        release(fp, envt, path, stdins, strbuf, 0, NULL);

        return 9;
    } else {
        lines = p_count_file_lines(cowbuf);     /* count file lines */
    }

    /* 
     * strunesc():    remove escape sequence
     * strlftonull(): rf to null
     */
    for (i = 0; i < stdins; i++) {
        strlftonull(strbuf[i]);
        strunesc(strbuf[i]);
    }
    for (i = 0; i < lines; i++) {
        strlftonull(cowbuf[i]);
    }

    /* print string */
    print_string(stdins, strbuf);
    /* print cow */
    print_cow(lines, cowbuf, &clsay);
    /* memory release */
    release(fp, envt, path, stdins, strbuf, lines, cowbuf);

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
        envt = NULL;
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
