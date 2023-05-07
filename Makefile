include config.mk

CC=cc
INSTALL=install
CFLAGS=-Wall -Wextra -std=c99 -Wpedantic -O3 -ggdb
BINARY=bswap
DIST=$(PREFIX)/bin/bswap

.PHONY: all install dist uninstall
all: $(BINARY)
install: $(DIST)
dist: $(DIST)
uninstall: distclean

$(BINARY): $(BINARY).c
	$(CC) $(CFLAGS) $< -o $@

$(DIST): $(BINARY)
	install -D $(BINARY) $@

distclean:
	rm -i $(DIST)

clean:
	rm $(BINARY)
