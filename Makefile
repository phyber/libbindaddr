LIBNAME=libbindaddr
CC?=gcc
FLAGS=-std=c99
CFLAGS=-Wall -fPIC -pedantic
LDFLAGS=-shared -ldl

all: clean $(LIBNAME)

clean:
	rm -f $(LIBNAME).so

debug: CFLAGS += -DDEBUG
debug: all

$(LIBNAME):
	$(CC) $(FLAGS) $(CFLAGS) $(LDFLAGS) -o $@.so $@.c
