#
#    Makefile for clangsay
#

WITH_SHARED	= 0
WITH_GLIB	= 1
WITH_REGEX	= 1
ifeq ($(shell uname), Linux)
WITH_GNU	= 1
endif
DEFAULT_COWFILE	=
TH_LINES	=
TH_LENGTH	=
