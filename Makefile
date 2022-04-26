SHELL=/bin/bash
CC=gcc
CFLAGS=-Wall
LDFLAGS=-I /usr/include -lX11 -lxkbfile
INSTALL=install
INSTALL_PROGRAM=$(INSTALL)
prefix=/usr/local

BUILD_DIR=build
SOURCES=main.c
OBJECTS=$(addprefix $(BUILD_DIR)/,$(SOURCES:.c=.o))
PROGRAM=xkbklu

all: prepare build

prepare:
	mkdir -p $(BUILD_DIR)

build: $(BUILD_DIR)/$(PROGRAM)

$(BUILD_DIR)/$(PROGRAM): $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SOURCES)
	$(CC) -o $@ -c $(CFLAGS) $<

install:
	$(INSTALL_PROGRAM) -D -t $(DESTDIR)$(prefix)/bin $(BUILD_DIR)/$(PROGRAM)

uninstall:
	rm $(DESTDIR)$(prefix)/bin/$(PROGRAM)

clean:
	rm -rf $(BUILD_DIR)
