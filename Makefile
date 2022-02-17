CC = gcc
FLAGS = -ggdb3 -Wall -Wextra -Wshadow -std=gnu11 -Wno-unused-parameter

# Linking ncurses works differently on Linux and Mac. Detect
# OS to account for this
HOST_SYSTEM = $(shell uname | cut -f 1 -d_)
SYSTEM ?= $(HOST_SYSTEM)
ifeq ($(SYSTEM),Darwin)
LIBS = -lncurses
FLAGS += -D_XOPEN_SOURCE_EXTENDED
else
LIBS = $(shell ncursesw5-config --libs)
FLAGS += $(shell ncursesw5-config --cflags)
endif

FILES = $(wildcard *.c) $(wildcard *.h)
OBJS = make_render.o
BINS = make_level

all: $(BINS)

# wildcard rule for compiling object file from source and header
%.o: %.c %.h
	$(CC) $(FLAGS) -c $< -o $@

make_level: $(OBJS) make_level.c
	$(CC) $(FLAGS) $^ $(LIBS) -o $@ -lm

clean:
	rm -f $(BINS)
	rm -f ${OBJS}
