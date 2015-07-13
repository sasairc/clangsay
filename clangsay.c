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
    char*   env     = NULL, /* $COWPATH */
        *   path    = NULL, /* .cow file */
        **  cowbuf  = NULL, /* string buffer (cow) */
        **  strbuf  = NULL; /* string buffer (string) */
    struct  stat    st;     /* file status */

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

    /* catenate file path */
    if ((env = getenv("COWPATH")) != NULL) {
        if (clsay.fflag == true) {
            path = strlion(3, env, "/", clsay.farg);
        } else {
            path = strlion(3, env, "/", DEFAULT_COWFILE);
        }
        if (path == NULL) {
            fprintf(stderr, "%s: strlion() failure\n", PROGNAME);

            return 1;
        }
    } else {
        if (clsay.fflag == true) {
            path = strlion(2, COWPATH, clsay.farg);
        } else {
            path = strlion(2, COWPATH, DEFAULT_COWFILE);
        }
        if (path == NULL) {
            fprintf(stderr, "%s: strlion() failure\n", PROGNAME);

            return 1;
        }
    }

    /* checking type of file or directory */
    if (stat(path, &st) != 0) {
        path = strlion(2, path, ".cow");        /* filename + .cow */
        if (stat(path, &st) != 0) {
            if (stat(clsay.farg, &st) == 0) {   /* filename */
                strcpy(path, clsay.farg);
            } else {
                fprintf(stderr, "%s: %s: no such file or directory\n", PROGNAME, path);
                release(NULL, path, 0, NULL, 0, NULL);

                return 2;
            }
        }
    }
    if (check_file_stat(path, st.st_mode) != 0) {
        release(NULL, path, 0, NULL, 0, NULL);

        return 3;
    }
    if ((fp = open_file(path)) == NULL) {
        release(NULL, path, 0, NULL, 0, NULL);

        return 4;
    }

    /* 
     * reading stdin or args to array
     * true : arguments
     * false: pipe
     */ 
    if (optind < argc) {    
        strbuf = (char**)malloc(sizeof(char*) * (argc - optind));   /* allocate array for y coordinate (strings) */
        if (strbuf == NULL) {
            fprintf(stderr, "%s: malloc() failure\n", PROGNAME);
            release(fp, path, 0, NULL, 0, NULL);
            
            return 5;
        }
        for (i = 0; optind < argc; optind++, i++) {
            strbuf[i] = (char*)malloc(sizeof(char) * (strlen(argv[optind]) + 1));
            if (strbuf[i] == NULL) {
                fprintf(stderr, "%s: malloc() failure\n", PROGNAME);
                release(fp, path, stdins, strbuf, 0, NULL);

                return 6;
            }
            strcpy(strbuf[i], argv[optind]);
        }
        stdins = i;
    } else {
        if ((strbuf = p_read_file_char(TH_LINES, TH_LENGTH, stdin)) == NULL) {
            fprintf(stderr, "%s: p_read_file_char() failure\n", PROGNAME);
            release(fp, path, 0, NULL, 0, NULL);

            return 7;
        }
        stdins = p_count_file_lines(strbuf);    /* count file lines */
    }

    /* reading cow file to array */
    if ((cowbuf = p_read_file_char(TH_LINES, TH_LENGTH, fp)) == NULL) {
        fprintf(stderr, "%s: p_read_file_char() failure\n", PROGNAME);
        release(fp, path, stdins, strbuf, 0, NULL);

        return 8;
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
    release(fp, path, stdins, strbuf, lines, cowbuf);

    return 0;
}

void release(FILE* fp, char* path, int lines1, char** buf1, int lines2, char** buf2)
{
    if (fp != NULL) {
        fclose(fp);
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
}
