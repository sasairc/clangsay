/*
 * clangsay -  The classic cowsay program, written in C.
 *
 * msg.c
 *
 * Copyright (c) 2015 sasairc
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar.HocevarHocevar See the COPYING file or http://www.wtfpl.net/
 * for more details.
 */

#include "./config.h"
#include "./msg.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifdef  WITH_SHARED
#include <benly/memory.h>
#include <benly/string.h>
#include <benly/file.h>
#else
#include <libbenly/src/memory.h>
#include <libbenly/src/string.h>
#include <libbenly/src/file.h>
/* WITH_SHARED */
#endif

#ifdef  WITH_REGEX
#include <regex.h>
/* WITH_REGEX */
#endif

static int read_msg(MSG** msg, int argc, int optind, char** argv);
static int print_msg(MSG* msg);
static void release_msg(MSG* msg);
static void strunsecs(MSG** msg);

int init_msg(MSG** msg)
{
    int     status  = 0;

    MSG*    tmp     = NULL;

    if ((tmp = (MSG*)
                malloc(sizeof(MSG))) == NULL) {
        status = -1; goto ERR;
    }

    tmp->data       = NULL;
    tmp->lines      = 0;
    tmp->read       = read_msg;
    tmp->print      = print_msg;
    tmp->release    = release_msg;
    *msg = tmp;

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
int read_msg(MSG** msg, int argc, int optind, char** argv)
{
    int     status  = 0;

    if (optind < argc) {    
        if (((*msg)->data = (char**)
                    smalloc(sizeof(char*) * (argc - optind), NULL)) == NULL) {
            status = -1; goto ERR;
        }
        while (optind < argc) {
            if ((*((*msg)->data + (*msg)->lines) = (char*)
                    smalloc(sizeof(char) * (strlen(*(argv + optind)) + 1), NULL)) == NULL) {
                status = -2; goto ERR;
            }
            memcpy(*((*msg)->data + (*msg)->lines),
                    *(argv + optind), strlen(*(argv + optind)) + 1);
            (*msg)->lines++;
            optind++;
        }
    } else {
        if (((*msg)->lines =
                    p_read_file_char(&(*msg)->data, TH_LINES, TH_LENGTH, stdin, 1)) < 0) {
            status = -3; goto ERR;
        }
    }
    /* remove escape sequence */
    strunsecs(msg);

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

static
int print_msg(MSG* msg)
{
    int         i       = 0,
                j       = 0,
                len     = 0,
                maxlen  = 0;    /* get max length */

#ifdef  WITH_REGEX
    regex_t     reg;

    /* compile regex */
    regcomp(&reg, ANSI_ESCSEQ, REG_EXTENDED);
    /* get max length */
    maxlen = strmax_with_regex(msg->lines, msg->data, &reg);
#else
    maxlen = strmax(msg->lines, msg->data);
/* WITH_REGEX */
#endif

    /*
     * single line
     */
    putchar(' ');
    i = maxlen;
    while (i >= 0) {
        putchar('_');
        i--;
    }
    if (msg->lines == 1) {
        fprintf(stdout, "\n< %s >\n ",
                *(msg->data));

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
    while (i < msg->lines) {
#ifdef  WITH_REGEX
        len = mbstrlen_with_regex(*(msg->data + i), &reg);
#else
        len = mbstrlen(*(msg->data + i));
/* WITH_REGEX */
#endif
        if (i == 0)
            fprintf(stdout, "\n/ %s",
                    *(msg->data + i));
        else if (i == (msg->lines - 1))
            fprintf(stdout, "\\ %s",
                    *(msg->data + i));
        else
            fprintf(stdout, "| %s",
                    *(msg->data + i));

        j = maxlen - len;
        while (j > 0) {
            putchar(' ');
            j--;
        }

        if (i == 0)
            puts(" \\");
        else if (i == (msg->lines - 1))
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
#ifdef  WITH_REGEX
    regfree(&reg);
/* WITH_REGEX */
#endif

    return 0;
}

static
void release_msg(MSG* msg)
{
    int     i   = 0,
            j   = 0;

    if (msg != NULL) {
        if (msg->data != NULL) {
            i = 0;
            j = msg->lines - 1;
            while (i <= j) {
                if (*(msg->data + i) != NULL) {
                    free(*(msg->data + i));
                    *(msg->data + i) = NULL;
                }
                if (*(msg->data + j) != NULL) {
                    free(*(msg->data + j));
                    *(msg->data + j) = NULL;
                }
                i++;
                j--;
            }
            free(msg->data);
            msg->data = NULL;
        }
        free(msg);
        msg = NULL;
    }

    return;
}

static
void strunsecs(MSG** msg)
{
    int i   = 0,
        j   = (*msg)->lines - 1;

    while (i <= j && i < (*msg)->lines) {
        strunesc(*((*msg)->data + i));
        strunesc(*((*msg)->data + j));
        i++;
        j--;
    }

    return;
}
