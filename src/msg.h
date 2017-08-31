/*
 * clangsay -  The classic cowsay program, written in C.
 *
 * msg.h
 *
 * Copyright (c) 2015 sasairc
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar.HocevarHocevar See the COPYING file or http://www.wtfpl.net/
 * for more details.
 */

#ifndef MSG_H
#define MSG_H
#ifdef  __cplusplus
extern "C" {
/* __cplusplus */
#endif

typedef struct MSG {
    char**  data;
    int     lines;
    int     (*read)(struct MSG** msg, int argc, int optind, char** argv);
    int     (*recursive)(struct MSG* msg, int n);
    int     (*print)(struct MSG* msg);
    void    (*release)(struct MSG* msg);
} MSG;

extern int init_msg(MSG** msg);

#ifdef  __cplusplus
}
/* __cplusplus */
#endif
/* MSG_H */
#endif
