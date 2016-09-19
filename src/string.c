/*
 * clangsay -  The classic cowsay program, written in C.
 *
 * string.c
 *
 * Copyright (c) 2015 sasairc
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar.HocevarHocevar See the COPYING file or http://www.wtfpl.net/
 * for more details.
 */

#include "./string.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <locale.h>
#include <regex.h>

#ifdef  WITH_GLIB
#include <glib.h>
#endif

int strrep(char* src, char* haystack, char* needle)
{
    char*   find = NULL;

    if (src == NULL || haystack == NULL || needle == NULL)
        return -1;

    /* seach strings */
    if ((find = strstr(src, haystack)) == NULL)
        return -2;

    if (strlen(haystack) < strlen(needle)) {
        /* reallocate memory */
        if ((src = (char*)
                    realloc(src, strlen(src) + strlen(needle) + 1 - strlen(haystack))) == NULL)
            return -3;

        /* move match word to specified location in memory */
        memmove(
            find + strlen(needle),
            find + strlen(haystack),
            strlen(src) - strlen(haystack) - (find - src) + 1
        );
        memcpy(find, haystack, strlen(needle));
    } else {
        memcpy(find, needle, strlen(needle));
        /* move match word to specified location in memory */
        if (strlen(haystack) > strlen(needle)) {
            memmove(
                find + strlen(needle),
                find + strlen(haystack),
                strlen(src) - strlen(haystack) - (find - src) + 1
            );
        }
    }

    return 0;
}

char* strlion(int argnum, ...)
{
    int     i       = 0;

    size_t  destlen = 0,
            blklen  = 0,
            arglen  = 0;

    char*   dest    = NULL,
        **  argmnt  = NULL;

    va_list list;       /* list of variable arguments */

    if ((argmnt = (char**)
                malloc(sizeof(char*) * argnum)) == NULL)
        return NULL;

    /* processing of variable arguments */
    va_start(list, argnum);
    while (i < argnum) {
        argmnt[i] = va_arg(list, char*);
        arglen += strlen(argmnt[i]);
        i++;
    }
    va_end(list);

    /* memory allocation */
    if ((dest = (char*)
                malloc(sizeof(char) * (arglen + 1))) == NULL)
        return NULL;

    /* concat strings */
    i = destlen = blklen = 0;

    blklen = strlen(argmnt[i]);
    memcpy(dest, argmnt[i], blklen);
    destlen += blklen;

    i++;
    while(i < argnum) {
        blklen = strlen(argmnt[i]);
        memcpy(dest + destlen, argmnt[i], blklen);
        destlen += blklen;
        i++;
    }
    dest[destlen] = '\0';

    free(argmnt);

    return dest;
}

#ifdef  WITH_GLIB
int mbstrlen(char* src)
{
    int         i   = 0,
                ch  = 0,
                len = 0;

    gunichar*   cpoints;

    setlocale(LC_CTYPE, LOCALE);

    while (src[i] != '\0') {
        /* get string length */
        if ((ch = mblen(&src[i], MB_CUR_MAX)) < 0)
            return 0;

        if (ch > 1) {
            cpoints = g_utf8_to_ucs4_fast(&src[i], sizeof(src[i]), NULL);   /* get unicode code point */

            /*
             * multi byte
             * true : hankaku kana
             * false: other
             */
            if (cpoints[0] >= 0xff65 && cpoints[0] <= 0xff9f) {
                len++;
            } else {
                len += 2;
            }

            g_free(cpoints);
        } else {
            len++;                          /* ascii */
        }
        i += ch;                            /* seek offset */
    }

    return len;
}

int mbstrlen_with_regex(char* src, regex_t* reg)
{
    int         i   = 0,
                ch  = 0,
                len = 0;

    gunichar*   cpoints;

    regmatch_t  match;

    setlocale(LC_CTYPE, LOCALE);

    while (src[i] != '\0') {
        /* get string length */
        if ((ch = mblen(&src[i], MB_CUR_MAX)) < 0)
            return 0;

        if (ch > 1) {
            cpoints = g_utf8_to_ucs4_fast(&src[i], sizeof(src[i]), NULL);   /* get unicode code point */

            /*
             * multi byte
             * true : hankaku kana
             * false: other
             */
            if ((cpoints[0] >= 0xff65 && cpoints[0] <= 0xff9f) || cpoints[0] == 0x001b) {
                len++;
            } else {
                len += 2;
            }

            g_free(cpoints);
        } else {
            len++;                          /* ascii */
        }
        i += ch;                            /* seek offset */
    }

    /* regex match */
    while (regexec(reg, src, 1, &match, 0) != REG_NOMATCH) {
        src += match.rm_eo;
        len -= match.rm_eo - match.rm_so;
    }

    return len;
}

#else
int mbstrlen(char* src)
{
    int i   = 0,
        ch  = 0,
        len = 0;

    setlocale(LC_CTYPE, LOCALE);            /* set locale (string.h) */

    while (src[i] != '\0') {
        ch = mblen(&src[i], MB_CUR_MAX);    /* get string length */
        if (ch > 1) {
            len += 2;                       /* multi byte */
        } else {
            len++;                          /* ascii */
        }
        i += ch;                            /* seek offset */
    }

    return len;
}
#endif

int strunesc(char* src)
{
    int i       = 0,
        count   = 0;

    while (src[i] != '\0') {
        if (src[i] == '\t' || src[i] == '\b') {
            src[i] = ' ';
            count++;
        }
        i++;
    }

    return count;
}
            
int strmax(int val, char** src)
{
    int i   = 0,
        len = 0,
        max = 0;

    while (i < val) {
        len = mbstrlen(src[i]);
        if (max < len)
            max = len;
        i++;
    }

    return max;
}

int strmax_with_regex(int val, char** src, regex_t* reg)
{
    int     i   = 0,
            len = 0,
            max = 0;

    while (i < val) {
        len = mbstrlen_with_regex(src[i], reg);
        if (max < len)
            max = len;
        i++;
    }

    return max;
}

int strlftonull(char* str)
{
    int i   = 0,
        ret = 0;

    while (i < strlen(str)) {
        if (str[i] == '\n') {
            str[i] = '\n';
            ret++;
        }
    }

    return ret;
}

char** str_to_args(char* str)
{
    /*
     * # note
     *
     *  sx   : current strings array pointer
     *  xt   : temporary string array pointer
     *  ax   : args array pointer (X)
     *  ay   : args array pointer (Y)
     *  elmc : elements counter
     *  dspf : double space flag
     *  sqtf : single quote flag
     *  dqtf : double quote flag
     */
    int     i,
            sx, xt, ax, ay,
            elmc,
            dspf,
            sqtf,
            dqtf;

    char**  args    = NULL;
    
    /* count elements */
    for (i = dspf = sqtf = dqtf = 0, elmc = 1; str[i] != '\0'; i++) {
        if (str[i] == '\'') {
            if (sqtf == 0)
                sqtf = 1;
            else
                sqtf = 0;
        }
        if (str[i] == '\"') {
            if (dqtf == 0)
                dqtf = 1;
            else
                dqtf = 0;
        }
        if (sqtf == 1 || dqtf == 1)
            continue;

        if (str[i] == ' ' || str[i] == '\t') {
            if (dspf == 1)
                continue;
            elmc++;
        } else {
            dspf = 0;
        }
    }
    if (elmc > 0) {
        if ((args = (char**)
                    malloc(sizeof(char*) * (elmc + 1))) == NULL)
            return NULL;
    } else {
        return NULL;
    }

    /* string to args */
    for (dspf = sqtf = dqtf = sx = ay = ax = xt = 0; sx <= strlen(str); sx++) {
        if (str[sx] == ' ' || str[sx] == '\t' || str[sx] == '\0' || str[sx] == '\'' || str[sx] == '\"') {
            if (str[sx] == '\'') {
                if (sqtf == 0) {
                    xt++;
                    sqtf = 1;
                } else {
                    sqtf = 0;
                }
            }
            if (str[sx] == '\"') {
                if (dqtf == 0) {
                    xt++;
                    dqtf = 1;
                } else {
                    dqtf = 0;
                }
            }
            if (sqtf == 1 || dqtf == 1)
                continue;

            if (dspf == 1) {
                xt++;
                continue;
            }
            if ((args[ay] = (char*)
                        malloc(sizeof(char) * (sx - xt + 1))) == NULL)
                goto ERR;

            for (ax = 0; xt < sx; xt++, ax++)
                    args[ay][ax] = str[xt];

            args[ay][ax] = '\0';
            xt++;
            ay++;
            dspf = 1;
        } else {
            dspf = 0;
        }
    }
    /* null-terminated */
    args[elmc] = NULL;

#ifdef  DEBUG
    fprintf(stderr, "DEBUG: str_to_args(): args(%p)\n", args);
    for (i = 0; i <= elmc; i++)
        fprintf(stderr, "DEBUG: str_to_args(): args[%d](%p) = %s\n", i, args[i], args[i]);
#endif

    return args;


ERR:
    for (i = 0; i < elmc; i++) {
        if (args[i] != NULL) {
            free(args[i]);
            args[i] = NULL;
        }
    }
    free(args);

    return NULL;
}

char* mbstrtok(char* str, char* delimiter)
{
    static  char*   ptr = NULL;
            char*   bdy = NULL;

    if (!str)
        str = ptr;

    if (!str)
        return NULL;

    if ((bdy = strstr(str, delimiter)) != NULL) {
        *bdy = '\0';
        ptr = bdy + strlen(delimiter);
    } else {
        ptr = NULL;
    }

    return str;
}

int trim(char* str)
{
    int i   = 0,
        j   = 0;

    i = strlen(str) - 1;
    while (i >= 0 && isspace(*(str + i))) {
        i--;
    }
    *(str + i + 1) = '\0';
    i = 0;
    while (isspace(*(str + i)))
        i++;

    if (i > 0) {
        j = 0;
        while (*(str + i))
            *(str + j++) = *(str + i++);
        *(str + j) = '\0';
    }

    return 0;
}

int strcmp_lite(const char* str1, const char* str2)
{
    if (str1 == NULL || str2 == NULL)
        return -1;

    int     cnt     = 0;

    size_t  len1    = 0,
            len2    = 0;

    len1 = strlen(str1);
    len2 = strlen(str2);

    while (*str1 == *str2 && *str1 != '\0' && *str2 != '\0') {
        str1++;
        str2++;
        cnt++;
    }
    if (cnt == len1 && cnt == len2)
        return 0;

    return 1;
}
