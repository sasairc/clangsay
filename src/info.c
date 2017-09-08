/*
 * clangsay -  The classic cowsay program, written in C.
 *
 * info.c
 *
 * Copyright (c) 2015 sasairc
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar.HocevarHocevar See the COPYING file or http://www.wtfpl.net/
 * for more details.
 */

#include "./config.h"
#include "./info.h"
#include <stdio.h>
#include <stdlib.h>

int print_version(void)
{
    fprintf(stdout, "%s %d.%d.%d%s (%s)\n",
            PROGNAME, VERSION, PATCHLEVEL, SUBLEVEL, EXTRAVERSION, ARCH);

    exit(0);
}

int print_usage(void)
{
    fprintf(stdout, "\
%s %d.%d.%d%s, The classic cowsay program, written in C.\n\
Usage: clangsay [OPTION]...\n\
\n\
Mandatory arguments to long options are mandatory for short options too.\n\
\n\
  -b,  --borg                borg mode\n\
  -d,  --dead                dead mode\n\
  -g,  --greedy              greedy mode\n\
  -p,  --paranoid            paranoid mode\n\
  -s,  --stoned              stoned mode\n\
  -t,  --tired               tired mode\n\
  -w,  --wired               wired mode\n\
  -y,  --youthful            youthful mode\n\
  -e,  --eye=EYE             manually specifies eye\n\
  -T,  --tongue=TONGUE       manually specifies tongue\n\
  -f,  --file=COWFILE        select cow file\n\
  -R,  --recursive=INT       recursive write message box\n\
  -l,  --list                display COWPATH directory and exit\n\
\n\
       --say                 speaking mode (default)\n\
       --think               thinking mode\n\
\n\
       --help                display this help and exit\n\
       --version             optput version infomation and exit\n\
\n\
Report %s bugs to %s <%s>\n\
",
        PROGNAME, VERSION, PATCHLEVEL, SUBLEVEL, EXTRAVERSION,
        PROGNAME, AUTHOR, MAIL_TO);

    exit(0);
}
