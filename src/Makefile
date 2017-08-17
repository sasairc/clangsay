#
#    Makefile for clangsay
#

SRCS       = $(wildcard *.c)
OBJS       = $(SRCS:.c=.o)
DEPOBJS    = ./libbenly/src/*.o
ARCH       = $(shell gcc -print-multiarch)
DEFLDFLAGS = `pkg-config --cflags --libs glib-2.0`
DEFCFLAGS  = -DPREFIX=\"$(PREFIX)\"	  \
		-DCOWPATH=\"$(COWPATH)/\" \
		-DARCH=\"$(ARCH)\"	  \
		-DLOCALE=\"\"             \
		-DWITH_GLIB               \
		-DWITH_REGEX
WITH_GLIB  = 1
WITH_REGEX = 1
export

all: $(TARGET) $(OBJS)

$(TARGET): build-dep $(OBJS)
ifeq ($(CMDLINE), 0)
	@echo "  BUILD   $@"
	@$(CC) $(LDFLAGS) $(OBJS) $(DEPOBJS) -o $@ $(DEFLDFLAGS)
else
	$(CC) $(LDFLAGS) $(OBJS) $(DEPOBJS) -o $@ $(DEFLDFLAGS)
endif

%.o: %.c %.h
ifeq ($(CMDLINE), 0)
	@echo "  CC      $@"
	@$(CC) $(CFLAGS) -c $< -o $@ $(DEFCFLAGS)
else
	$(CC) $(CFLAGS) -c $< -o $@ $(DEFCFLAGS)
endif

build-dep:
	@$(MAKE) -C ./libbenly	all

clean-dep:
	@$(MAKE) -C ./libbenly	clean

install-bin: $(TARGET)
	install -pd $(BINDIR)
	install -pm 755 $< $(BINDIR)/

clean: clean-dep
	-$(RM) -f $(OBJS)
	-$(RM) -f $(TARGET)

.PHONY: all		\
	build-dep	\
	clean-dep	\
	install-bin	\
	clean
