/*
 * clangsay -  The classic cowsay program, written in C.
 *
 * list.c
 *
 * Copyright (c) 2015 sasairc
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar.HocevarHocevar See the COPYING file or http://www.wtfpl.net/
 * for more details.
 */

#include "./config.h"
#include "./list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#ifdef  WITH_SHARED
#include <benly/env.h>
#else
#include <libbenly/src/env.h>
/* WITH_SHARED */
#endif

static int selects_cowfiles(const struct dirent* dir);

static
int selects_cowfiles(const struct dirent* dir)
{
    char*   tcow  = ".cow\0";

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
