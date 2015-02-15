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
#include "./string.h"
#include "./file.h"
#include "./memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <dirent.h>
#include <sys/stat.h>

int main(int argc, char* argv[])
{
    int     i;
    int     res, index;     /* use getopt_long() */
    int     lines  = 0;     /* lines of cowfile*/
    int     stdins = 0;     /* lines of string */
    char*   env    = NULL;  /* $COWPATH */
    char*   path   = NULL;  /* .cow file */
    char**  cowbuf = NULL;  /* string buffer (cow) */
    char**  strbuf = NULL;  /* string buffer (string) */
    FILE*   fp     = NULL;  /* cow-file */
    struct  stat st;        /* file status */

    /* flag and args */
    clangsay_t  clsay = {
        false, false, false, false, false, false, false, false, false, false, false, false,
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
        {"help",     no_argument,       NULL, 'h'},
        {"version",  no_argument,       NULL, 'v'},
        {0, 0, 0, 0},
    };

    /* processing of arguments */
    while ((res = getopt_long(
                    argc,
                    argv,
                    "nW;bdgpstwye:T:f:lhv",
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
                break;
            case    'h':
                print_usage();
                break;
            case    'v':
                fprintf(stdout, "%s %d.%d.%d\n", PROGNAME, VERSION, PATCHLEVEL, SUBLEVEL);
                exit(0);
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
            fprintf(stderr, "%s: strlion() failed\n", PROGNAME);

            return 1;
        }
    } else {
        if (clsay.fflag == true) {
            path = strlion(2, COWPATH, clsay.farg);
        } else {
            path = strlion(2, COWPATH, DEFAULT_COWFILE);
        }
        if (path == NULL) {
            fprintf(stderr, "%s: strlion() failed\n", PROGNAME);

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
                free(path);

                return 2;
            }
        }
    }
    if ((st.st_mode & S_IFMT) == S_IFDIR) {
        fprintf(stderr, "%s: %s: is a directory\n", PROGNAME, path);
        free(path);
        
        return 3;
    }

    /* checking file permission */
    if (access(path, R_OK) != 0) {
        fprintf(stderr, "%s: %s: permission denied\n", PROGNAME, path);
        free(path);

        return 4;
    }

    /* open after checking file type */
    if (check_file_type(path) == 0) {
        fp = fopen(path, "r");
    } else {
        fprintf(stderr, "%s: %s: unknown file type\n", PROGNAME, path);
        free(path);

        return 5;
    }
    if (fp == NULL) {
        fprintf(stderr, "%s: fp is NULL\n", PROGNAME);
        free(path);

        return 6;
    }

    lines = count_file_lines(fp);                           /* count line for cow-file */
    cowbuf = (char**)malloc(sizeof(char*) * (lines + 1));   /* allocate array for y coordinate (cows) */
    strbuf = (char**)malloc(sizeof(char*) * STLINE);        /* allocate array for y coordinate (strings) */

    if (cowbuf == NULL || strbuf == NULL) {
        fprintf(stderr, "%s: malloc() failed\n", PROGNAME);
        fclose(fp);
        free(path);

        return 7;
    }

    /* reading stdin or args to array */
    if (argv[optind] != NULL) {
        for (i = 0; optind < argc && i < STLINE; optind++, i++) {
            strbuf[i] = (char*)malloc(sizeof(char) * (strlen(argv[optind]) + 1));
            if (strbuf[i] == NULL) {
                fprintf(stderr, "%s: malloc() failed\n", PROGNAME);
                fclose(fp);
                free(path);
                free2d(strbuf, i);

                return 8;
            }
            strcpy(strbuf[i], argv[optind]);
        }
        stdins = i;
    } else {
        if ((stdins = read_file(STLINE, BUFLEN, strbuf, stdin)) == 0) {
            fprintf(
                stderr,
                "%s capacity of buffer is not enough: BUFLEN=%d\n",
                PROGNAME, BUFLEN
            );
            fclose(fp);
            free(path);
            free2d(strbuf, stdins);

            return 9;
        }
    }

    /* reading cow file to array */
    rewind(fp);
    if (read_file(lines, BUFLEN, cowbuf, fp) == 0) {
        fprintf(
                stderr,
                "%s capacity of buffer is not enough: BUFLEN=%d\n",
                PROGNAME, BUFLEN
        );
        fclose(fp);
        free(path);
        free2d(strbuf, stdins);
        free2d(cowbuf, (lines + 1));

        return 10;
    } else {
        fclose(fp);     /* close cow file */
    }

    /* remove escape sequence */
    for (i = 0; i < stdins; i++) {
        strunesc(strbuf[i]);
    }

    /* print string */
    print_string(stdins, strbuf);
    /* print cow */
    print_cow(lines, cowbuf, &clsay);

    free(path);                     /* release memory (cowfile path) */
    free2d(strbuf, stdins);         /* release memory (strings) */
    free2d(cowbuf, (lines + 1));    /* release memory (cow) */

    return 0;
}

int print_string(int lines, char** str)
{
    int i, j;
    int len = strmax(lines, str);       /* get max length */

    /* one line */
    fprintf(stdout, " ");
    for (i = 0; i <= len; i++)
        fprintf(stdout, "_");

    if (lines == 1) {
        fprintf(stdout, "\n< %s >\n ", str[0]);
        for (i = 0; i <= len; i++)
            fprintf(stdout, "-");

        fprintf(stdout, "\n");

        return 0;
    }

    /* multi line */
    for (i = 0; i < lines; i++) {
        if (i == 0) {
            fprintf(stdout, "\n/ %s", str[i]);
        } else if (i == (lines -1)) {
            fprintf(stdout, "\\ %s", str[i]);
        } else {
            fprintf(stdout, "| %s", str[i]);
        }

        for (j = 0; j < (len - mbstrlen(str[i])); j++) {
            fprintf(stdout, " ");
        }

        if (i == 0) {
            fprintf(stdout, " \\\n");
        } else if (i == (lines - 1)) {
            fprintf(stdout, " /\n ");
        } else {
            fprintf(stdout, " |\n");
        }
    }
    for (i = 0; i <= len; i++)
        fprintf(stdout, "-");

    fprintf(stdout, "\n");

    return 0;
}

int print_cow(int lines, char** str, clangsay_t* clsay)
{
    int     i;
    bool    block = false;

    /* print cow */
    for (i = 0; i < lines; i++) {
        strrep(str[i], THOUGHTS, DEFAULT_THOUGHTS);
        strrep(str[i], "\\\\", "\\");

        /* relace eyes*/
        if (clsay->eflag == true) {
            strrep(str[i], EYES, clsay->earg);
        } else if (clsay->bflag == true) {
            strrep(str[i], EYES, BORG_EYES);
        } else if (clsay->dflag == true) {
            strrep(str[i], EYES, DEAD_EYES);
        } else if (clsay->gflag == true) {
            strrep(str[i], EYES, GREEDY_EYES);
        } else if (clsay->pflag == true) {
            strrep(str[i], EYES, PARANOID_EYES);
        } else if (clsay->sflag == true) {
            strrep(str[i], EYES, STONED_EYES);
        } else if (clsay->tflag == true) {
            strrep(str[i], EYES, TIRED_EYES);
        } else if (clsay->wflag == true) {
            strrep(str[i], EYES, WIRED_EYES);
        } else if (clsay->yflag == true) {
            strrep(str[i], EYES, YOUTHFUL_EYES);
        } else {
            strrep(str[i], EYES, DEFAULT_EYES);
        }

        /* relace tongue*/
        if (clsay->Tflag == true) {
            strrep(str[i], TONGUE, clsay->Targ);
        } else if (clsay->dflag == true || clsay->sflag == true) {
            strrep(str[i], TONGUE, DEAD_TONGUE);
        } else {
            strrep(str[i], TONGUE, DEFAULT_TONGUE);
        } 

        /* EOC to EOC */
        if (strstr(str[i], "EOC")) {
            block = true;
            continue;
        } else if (strstr(str[i], "EOC") && block == true) {
            block = false;
        }
        if (block == true) {
            fprintf(stdout, "%s\n", str[i]);
        }
    }

    return 0;
}

int selects_cowfiles(const struct dirent* dir)
{
    int     namlen;
    int*    lp;
    char    dotcow[] = {".cow"};

    namlen = strlen(dir->d_name);
    if (namlen < 4) return 0;
    namlen -= 4;    /* offset 4 bytes from end (.cow) */

    lp = (int*)&(dir->d_name[namlen]);

    /*
     * comparison on int
     * true: .cow
     * false: other
     */
    if (*lp == *(int*)&dotcow)
        return 1;

    return 0;
}

int list_cowfiles(void)
{
    int     i;
    int     entry;
    char*   path    = NULL;
    struct  dirent**  list;

    /* catenate file path */
    if ((path = getenv("COWPATH")) == NULL) {
        path = COWPATH;
    }

    /* get file entry and sort */
    if ((entry = scandir(path, &list, selects_cowfiles, alphasort)) == -1) {
        fprintf(stderr, "%s: scandir() failed\n", PROGNAME);

        exit(11);
    }
    for (i = 0; i < entry; i++) {
        fprintf(stdout, "%s\n", list[i]->d_name);
        free(list[i]);
    }
    free(list);

    exit(0);
}

int print_usage(void)
{
    fprintf(stdout, "\
%s %d.%d.%d The classic cowsay program, written in C.\n\
Usage: clangsay [OPTION]...\n\
\n\
Mandatory arguments to long options are mandatory for short options too.\n\
\n\
  -b,  --borg                borg mode\n\
  -d,  --dead                dead mode\n\
  -g,  --greedy              greedy mode\n\
  -s,  --stoned              stoned mode\n\
  -t,  --tired               tired mode\n\
  -w,  --wired               wired mode\n\
  -y,  --youthful            youthful mode\n\
  -e,  --eyes=EYES           manually specifies eyes (DEFAULT=oo)\n\
  -T,  --tongue=TONGUE       manually specifies tongue\n\
  -f,  --file=COWFILE        select cow file\n\
  -l,  --list                display COWPATH directory and exit\n\
\n\
  -h,  --help                display this help and exit\n\
  -v,  --version             optput version infomation and exit\n\
\n\
Report %s bugs to %s <%s>\n\
", PROGNAME, VERSION, PATCHLEVEL, SUBLEVEL,
AUTHOR, AUTHOR, MAIL_TO);

    exit(0);
}
