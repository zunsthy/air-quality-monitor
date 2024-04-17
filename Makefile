VERSION := 1.0.0
APPNAME := air_quality
CC := gcc

SRCS := $(wildcard ./src/*.c)
MAIN := $(SRCS) ./src/main.c

$(APPNAME): $(MAIN)
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean

clean:
	rm $(APPNAME)
