#
#    Makefile for clangsay
#

TARGET	= clangsay
PREFIX	:= /usr/local
BINDIR	:= $(PREFIX)/bin
MANDIR	:= $(PREFIX)/share/man/ja/man6
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

all clean:
	@$(MAKE) -C ./src	$@
	@$(MAKE) -C ./compdef	$@

install-bin:
	@$(MAKE) -C ./src	$@

install-man:
	@$(MAKE) -C ./doc	$@

install-zsh-compdef:
	@$(MAKE) -C ./compdef	$@

install-cows:
	@$(MAKE) -C ./cows	$@

install: install-bin		\
	 install-man		\
	 install-cows		\
	 install-zsh-compdef

.PHONY: all			\
	install			\
	install-bin		\
	install-man		\
	install-cows		\
	install-zsh-compdef	\
	clean
