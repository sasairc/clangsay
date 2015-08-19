#
#    Makefile for clangsay
#

all: _$(TARGET).zsh

_$(TARGET).zsh: _$(TARGET).zsh.src
ifeq ($(CMDLINE), 0)
	@echo "  BUILD   $@"
	@cat $< | sed -e 's%_COWPATH%${COWPATH}%g' > $@
else
	cat $< | sed -e 's%_COWPATH%${COWPATH}%g' > $@
endif

install-zsh-compdef: _$(TARGET).zsh
	install -pd $(PREFIX)/share/$(TARGET)/compdef
	install -pm 644 $< $(PREFIX)/share/$(TARGET)/compdef/

clean:
	-$(RM) -f _$(TARGET).zsh

.PHONY: all			\
	install-zsh-compdef	\
	clean
