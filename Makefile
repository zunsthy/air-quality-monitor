VERSION := 1.0.0
APPNAME := air_quality
CC := gcc
LFLAGS := -lsqlite3

SRCS := $(wildcard ./src/*.c)
MAIN := $(SRCS) ./src/main.c

$(APPNAME): $(MAIN)
	$(CC) $(CFLAGS) $^ -o $@ $(LFLAGS)

.PHONY: clean

clean:
	rm $(APPNAME)
