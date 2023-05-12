include config.mk

CC=cc
INSTALL=install
CFLAGS=-Wall -Wextra -std=c99 -Wpedantic -O3 -ggdb
BINARY=bswap
TARGET=$(PREFIX)/bin/bswap
MANTRG=$(PREFIX)/share/man/man1/bswap.1

.PHONY: all install dist uninstall
all: $(BINARY)
dist: install
uninstall: distclean

$(BINARY): $(BINARY).c
	$(CC) $(CFLAGS) $< -o $@

install: $(BINARY)
	install -D $(BINARY) $(TARGET)
	install -D $(BINARY).1 $(MANTRG)

distclean:
	rm -i $(TARGET)

clean:
	rm $(BINARY)
