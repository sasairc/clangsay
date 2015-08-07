#
#    Makefile for clangsay
#

override \
CFLAGS	+= -fno-strict-aliasing

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
	@$(CC) $(LDFLAGS) $^ -o $@ $(DEFLDFLAGS)
else
	$(CC) $(LDFLAGS) $^ -o $@ $(DEFLDFLAGS)
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
	@cat $< | sed -e 's%_COWPATH%${COWPATH}%g' > $@
else
	cat $< | sed -e 's%_COWPATH%${COWPATH}%g' > $@
endif

install-bin: $(TARGET)
	install -pd $(BINDIR)
	install -pm 755 $< $(BINDIR)/

install-zsh-compdef: _$(TARGET).zsh
	install -pd $(PREFIX)/share/$(TARGET)/zsh
	install -pm 644 $< $(PREFIX)/share/$(TARGET)/zsh

install: install-bin install-zsh-compdef

clean:
	-$(RM) -f $(OBJS)
	-$(RM) -f $(TARGET)
	-$(RM) -f _$(TARGET).zsh

.PHONY: all install install-bin install-zsh-compdef clean
