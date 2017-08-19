#
#    Makefile for clangsay
#

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
WITH_SHARED = 0
# enable use of GLib (libglib-2.0) with the libbenly
WITH_GLIB   = 1
DEFCFLAGS   += -DWITH_GLIB
DEFLDFLAGS  += `pkg-config --cflags --libs glib-2.0`
# enable use of POSIX Regular Expressions (regex.h) with the libbenly
WITH_REGEX  = 1
DEFCFLAGS   += -DWITH_REGEX
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
endif

build-dep $(DEPOBJS):
	@$(MAKE) -C ./libbenly	all

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
