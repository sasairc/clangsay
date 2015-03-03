/*
 * clangsay -  The classic cowsay program, written in C.
 *
 * config.h
 *
 * Copyright (c) 2015 sasairc
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar.HocevarHocevar See the COPYING file or http://www.wtfpl.net/
 * for more details.
 */

#ifndef CONFIG_H
#define CONFIG_H

#define PROGNAME        "clangsay"
#define VERSION         0
#define PATCHLEVEL      0
#define SUBLEVEL        5
#define AUTHOR          "sasairc"
#define MAIL_TO         "sasairc@ssiserver.moe.hm"

#define BUFLEN          1024
#define STLINE          480

#define EYES            "$eyes"
#define THOUGHTS        "$thoughts"
#define TONGUE          "$tongue"

#define DEFAULT_COWFILE "default.cow"
#define DEFAULT_THOUGHTS    "\\"
#define DEFAULT_EYES    "oo"
#define DEFAULT_TONGUE  "  "

#define BORG_EYES       "=="
#define DEAD_EYES       "XX"
#define GREEDY_EYES     "$$"
#define PARANOID_EYES   "@@"
#define STONED_EYES     "**"
#define TIRED_EYES      "--"
#define WIRED_EYES      "OO"
#define YOUTHFUL_EYES   ".."

#define DEAD_TONGUE     "U "

#endif
