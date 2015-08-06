#
#    Makefile for clangsay
#

TARGET	= clangsay
PREFIX	:= /usr/local
BINDIR	:= $(PREFIX)/bin
COWPATH	:= $(PREFIX)/share/$(TARGET)/cows
INCLUDE :=
LIBS	:=
MAKE	:= make
CC	:= cc
RM	:= rm
CFLAGS	:= -O2 -g -Wall -fno-strict-aliasing
LDFLAGS	:=
CMDLINE	:= 0
export

all clean install-bin install-zsh-compdef:
	@$(MAKE) -C ./src	$@

install-cows:
	@$(MAKE) -C ./cows	$@

install: install-bin install-cows install-zsh-compdef

.PHONY: all install install-bin install-cows install-zsh-compdef clean
