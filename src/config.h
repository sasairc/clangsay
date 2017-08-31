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
#ifdef  __cplusplus
extern "C" {
/* __cplusplus */
#endif

#define PROGNAME        "clangsay"
#define VERSION         1
#define PATCHLEVEL      7
#define SUBLEVEL        0
#define EXTRAVERSION    "-devel"
#define AUTHOR          "sasairc"
#define MAIL_TO         "sasairc@ssiserver.moe.hm"

/*
 * # setting of realloc() timing
 * TH_LINES  : threshold of lines (sizeof(char*) * TH_LINES)
 * TH_LENGTH : threshold of string (sizeof(char) * TH_LENGTH)
 */
#ifndef TH_LINES
#define TH_LINES        256
/* TH_LINES */
#endif
#ifndef TH_LENGTH
#define TH_LENGTH       512
/* TH_LENGTH */
#endif

#define EYES            "$eyes"
#define THOUGHTS        "$thoughts"
#define TONGUE          "$tongue"

#ifndef DEFAULT_COWFILE
#define DEFAULT_COWFILE "default.cow"
/* DEFAULT_COWFILE */
#endif

#define DEFAULT_EYES    "oo"
#define DEFAULT_TONGUE  "  "

#define SAY_THOUGHTS    "\\"
#define THINK_THOUGHTS  "o"

#define BORG_EYES       "=="
#define DEAD_EYES       "XX"
#define GREEDY_EYES     "$$"
#define PARANOID_EYES   "@@"
#define STONED_EYES     "**"
#define TIRED_EYES      "--"
#define WIRED_EYES      "OO"
#define YOUTHFUL_EYES   ".."

#define DEAD_TONGUE     "U "

#ifdef  WITH_REGEX
#define ANSI_ESCSEQ     "\x1b\[[0-9;]*[a-zA-Z]"
/* WITH_REGEX */
#endif

#ifdef  __cplusplus
}
/* __cplusplus */
#endif
/* CONFIG_H */
#endif
