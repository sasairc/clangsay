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
PKGCFG	= `pkg-config --cflags --libs glib-2.0`
SRCS	= $(wildcard *.c)
OBJS	= $(SRCS:.c=.o)
ARCH	= $(shell gcc -print-multiarch)

DEFCFLAGS = -DPREFIX=\"$(PREFIX)\"	  \
		-DCOWPATH=\"$(COWPATH)/\" \
		-DARCH=\"$(ARCH)\"	  \
		$(INCLUDE)		  \
		$(LIBS)		 	  \
		$(PKGCFG)

DEFLDFLAGS = $(PKGCFG)

all: $(TARGET) $(OBJS) _$(TARGET).zsh

$(TARGET): $(OBJS)
ifeq ($(CMDLINE), 0)
	@echo "  BUILD   $@"
	@$(CC) $(LDFLAGS) $(OBJS) -o $(TARGET) $(DEFLDFLAGS)
else
	$(CC) $(LDFLAGS) $(OBJS) -o $(TARGET) $(DEFLDFLAGS)
endif

%.o: %.c %.h
ifeq ($(CMDLINE), 0)
	@echo "  CC      $@"
	@$(CC) $(DEFCFLAGS) $(CFLAGS) -c $< -o $@
else
	$(CC) $(DEFCFLAGS) $(CFLAGS) -c $< -o $@
endif

_$(TARGET).zsh: _$(TARGET).zsh.src
ifeq ($(CMDLINE), 0)
	@echo "  BUILD   $@"
	@cat _$(TARGET).zsh.src | sed -e 's%_COWPATH%${COWPATH}%g' > _$(TARGET).zsh
else
	cat _$(TARGET).zsh.src | sed -e 's%_COWPATH%${COWPATH}%g' > _$(TARGET).zsh
endif

install-bin: $(TARGET)
	install -pd $(BINDIR)
	install -pm 755 $(TARGET) $(BINDIR)/

install-cows:
	install -pd $(COWPATH)
	install -pm 644 ./cows/* $(COWPATH)/

install-zsh-compdef: _$(TARGET).zsh
	install -pd $(PREFIX)/share/$(TARGET)/zsh
	install -pm 644 _$(TARGET).zsh $(PREFIX)/share/$(TARGET)/zsh

install: install-bin install-cows install-zsh-compdef

clean:
	-$(RM) -f $(OBJS)
	-$(RM) -f $(TARGET)
	-$(RM) -f _$(TARGET).zsh

.PHONY: all install clean
