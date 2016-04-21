#
#    Makefile for yasuna
#

install-man: clangsay.6
ifeq ($(LANG),ja_JP.UTF-8)
	install -pd $(MANDIR)
	install -pm 644 $< $(MANDIR)/
endif

.PHONY: install-man
