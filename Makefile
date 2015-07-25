#
#    Makefile for clangsay
#

TARGET	= clangsay
PREFIX	:= /usr/local
BINDIR	:= $(PREFIX)/bin
COWPATH	:= $(PREFIX)/share/$(TARGET)/cows
INCLUDE :=
LIBS	:=
PKGCFG	:= `pkg-config --cflags --libs glib-2.0`
MAKE	:= make
CC	:= cc
RM	:= rm
CFLAGS	:= -O2 -g -Wall -fno-strict-aliasing
LDFLAGS	:=
SRCS	= $(wildcard *.c)
OBJS	= $(SRCS:.c=.o)
ARCH	:= $(shell gcc -print-multiarch)

DEFCFLAGS = -DPREFIX=\"$(PREFIX)\"	  \
		-DCOWPATH=\"$(COWPATH)/\" \
		-DARCH=\"$(ARCH)\"	  \
	    	$(INCLUDE)		  \
		$(LIBS)		 	  \
		$(PKGCFG)

DEFLDFLAGS = $(PKGCFG)

all: $(TARGET) $(OBJS) _$(TARGET).zsh

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(TARGET) $(DEFLDFLAGS)

clangsay.o: clangsay.c clangsay.h config.h
	$(CC) $(DEFCFLAGS) $(CFLAGS) -c clangsay.c -o clangsay.o

info.o: info.c info.h config.h
	$(CC) $(DEFCFLAGS) $(CFLAGS) -c info.c -o info.o

subset.o: subset.c subset.h config.h
	$(CC) $(DEFCFLAGS) $(CFLAGS) -c subset.c -o subset.o

file.o: file.c file.h
	$(CC) $(DEFCFLAGS) $(CFLAGS) -c file.c -o file.o

string.o: string.c string.h
	$(CC) $(DEFCFLAGS) $(CFLAGS) -c string.c -o string.o

memory.o: memory.c memory.h
	$(CC) $(DEFCFLAGS) $(CFLAGS) -c memory.c -o memory.o

_$(TARGET).zsh: _$(TARGET).zsh.src
	cat _$(TARGET).zsh.src | sed -e 's%_COWPATH%${COWPATH}%g' > _$(TARGET).zsh

install-bin: $(TARGET)
	install -pd $(BINDIR)
	install -pm 755 $(TARGET) $(BINDIR)/

install-cows:
	install -pd $(COWPATH)
	install -pm 644 ./cows/* $(COWPATH)/

install-zsh-compdef:
	install -pd $(PREFIX)/share/$(TARGET)/zsh
	install -pm 644 _$(TARGET).zsh $(PREFIX)/share/$(TARGET)/zsh

install: install-bin install-cows install-zsh-compdef

.PHONY: clean
clean:
	-$(RM) -f $(OBJS)
	-$(RM) -f $(TARGET)
	-$(RM) -f _$(TARGET).zsh
