#
#    Makefile for clangsay
#

TARGET	= clangsay
PREFIX	:= /usr/local
BINDIR	:= $(PREFIX)/bin
COWPATH	:= $(PREFIX)/share/clangsay/cows
MAKE	:= make
CC	:= cc
RM	:= rm
CFLAGS	:= -O2 -g -Wall -fno-strict-aliasing
LDFLAGS	:=
SRCS	= clangsay.c subset.c file.c string.c memory.c
OBJS	= $(SRCS:.c=.o)

all: $(TARGET) $(OBJS)

DEFCFLAGS = -DPREFIX=\"$(PREFIX)\"  \
		-DCOWPATH=\"$(COWPATH)/\"

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(TARGET)

clangsay.o: clangsay.c
	$(CC) $(DEFCFLAGS) $(CFLAGS) -c clangsay.c -o clangsay.o

subset.o: subset.c
	$(CC) $(DEFCFLAGS) $(CFLAGS) -c subset.c -o subset.o

file.o: file.c
	$(CC) $(DEFCFLAGS) $(CFLAGS) -c file.c -o file.o

string.o: string.c
	$(CC) $(DEFCFLAGS) $(CFLAGS) -c string.c -o string.o

memory.o: memory.c
	$(CC) $(DEFCFLAGS) $(CFLAGS) -c memory.c -o memory.o

install-bin: $(TARGET)
	install -pd $(BINDIR)
	install -pm 755 $(TARGET) $(BINDIR)/

install-cows:
	install -pd $(COWPATH)
	install -pm 644 ./cows/* $(COWPATH)/

install: install-bin install-cows

.PHONY: clean
clean:
	-$(RM) -f $(OBJS)
	-$(RM) -f $(TARGET)
