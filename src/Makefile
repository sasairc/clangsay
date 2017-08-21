#
#    Makefile for clangsay
#

include ../config.mk

SRCS        = $(wildcard *.c)
OBJS        = $(SRCS:.c=.o)
DEPSRCS     = $(wildcard ./libbenly/src/*.c)
DEPOBJS     = $(DEPSRCS:.c=.o)
ARCH        = $(shell gcc -print-multiarch)
DEFCFLAGS   = -DPREFIX=\"$(PREFIX)\"	  \
		-DCOWPATH=\"$(COWPATH)/\" \
		-DARCH=\"$(ARCH)\"	  \
		-DLOCALE=\"\"

# use shared libraries (default: disable)
WITH_SHARED ?= 0
# enable use of GLib (libglib-2.0) with the libbenly
ifeq ($(WITH_GLIB), 1)
WITH_GLIB   ?= 1
DEFCFLAGS   += -DWITH_GLIB
DEFLDFLAGS  += `pkg-config --cflags --libs glib-2.0`
# WITH_GLIB
endif
# enable use of POSIX Regular Expressions (regex.h) with the libbenly
ifeq ($(WITH_REGEX), 1)
WITH_REGEX  ?= 1
DEFCFLAGS   += -DWITH_REGEX
# WITH_REGEX
endif
# manually specifies default cowfile
ifdef DEFAULT_COWFILE
DEFCFLAGS   += -DDEFAULT_COWFILE=\"$(DEFAULT_COWFILE)\"
# DEFAULT_COWFILE
endif
# threshold of lines (sizeof(char*) * TH_LINES)
ifdef TH_LINES
DEFCFLAGS   += -DTH_LINES=$(TH_LINES)
# TH_LINES
endif
# threshold of string (sizeof(char) * TH_LENGTH)
ifdef TH_LENGTH
DEFCFLAGS   += -DTH_LENGTH=$(TH_LENGTH)
# TH_LENGTH
endif
export

all: $(TARGET)

ifeq ($(WITH_SHARED), 0)
$(TARGET): $(DEPOBJS) $(OBJS)
ifeq ($(CMDLINE), 0)
	@echo "  BUILD   $@"
	@$(CC) $(LDFLAGS) $^ -o $@ $(DEFLDFLAGS)
else
	$(CC) $(LDFLAGS) $^ -o $@ $(DEFLDFLAGS)
# CMDLINE
endif
else
DEFLDFLAGS += -lbenly_env -lbenly_file -lbenly_memory -lbenly_string
$(TARGET): $(OBJS)
ifeq ($(CMDLINE), 0)
	@echo "  BUILD   $@"
	@$(CC) $(LDFLAGS) $^ -o $@ $(DEFLDFLAGS)
else
	$(CC) $(LDFLAGS) $^ -o $@ $(DEFLDFLAGS)
# CMDLINE
endif
# WITH_SHARED
endif

%.o: %.c %.h
ifeq ($(CMDLINE), 0)
	@echo "  CC      $@"
	@$(CC) $(CFLAGS) -c $< -o $@ $(DEFCFLAGS)
else
	$(CC) $(CFLAGS) -c $< -o $@ $(DEFCFLAGS)
# CMDLINE
endif

build-dep $(DEPOBJS):
	@$(MAKE) -C ./libbenly	CFLAGS="$(CFLAGS)" all

clean-dep:
	@$(MAKE) -C ./libbenly	clean

install-dep:
	@$(MAKE) -C ./libbenly	install

install-bin: $(TARGET)
	install -pd $(BINDIR)
	install -pm 755 $< $(BINDIR)/

clean: clean-dep
	-$(RM) -f $(OBJS)
	-$(RM) -f $(TARGET)

.PHONY: all		\
	build-dep	\
	clean-dep	\
	install-dep	\
	install-bin	\
	clean
