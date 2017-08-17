#
#    Makefile for clangsay
#

TARGET	= clangsay
PREFIX	:= /usr/local
BINDIR	:= $(PREFIX)/bin
MANDIR	:= $(PREFIX)/share/man/ja/man6
COWPATH	:= $(PREFIX)/share/$(TARGET)/cows
MAKE	:= make
CC	:= cc
RM	:= rm
CFLAGS	:= -O2 -g -Wall
LDFLAGS	:=
CMDLINE	:= 0
DOCKER	:= docker
IMGTAG	:= latest
export

all clean:
	@$(MAKE) -C ./src	$@
	@$(MAKE) -C ./compdef	$@

build-dep clean-dep install-bin:
	@$(MAKE) -C ./src	$@

install-man:
	@$(MAKE) -C ./doc	$@

install-zsh-compdef:
	@$(MAKE) -C ./compdef	$@

install-cows install-cows-asciiart install-cows-pixelart:
	@$(MAKE) -C ./cows	$@

install: install-bin		\
	 install-man		\
	 install-cows		\
	 install-zsh-compdef

docker-image:
	@$(DOCKER) build -t clangsay:$(IMGTAG) .

.PHONY: all			\
	build-dep               \
	clean-dep               \
	install			\
	install-bin		\
	install-man		\
	install-cows		\
	install-cows-asciiart	\
	install-cows-pixelart	\
	install-zsh-compdef	\
	clean			\
	docker-image
