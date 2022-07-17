CC=gcc
CFLAGS=-ansi -iquote include -pedantic-errors -Wall -Wextra
DEBUG=-g
RELEASE=-DNDEBUG -O3
LDEBUG=libdsdebug.so
LRELEASE=libdsrelease.so
SHARED=-fPIC -shared
RPATH=-Wl,-rpath="\$$ORIGIN"
LINKED=-ldsdebug -L. $(RPATH) -lpthread
SRCS:=$(wildcard source/*.c)

all: debug release

test: debug

debug:
	$(CC) $(SHARED) $(CFLAGS) $(DEBUG) $(SRCS) -lm -o $(LDEBUG)

release:
	$(CC) $(SHARED) $(CFLAGS) $(RELEASE) $(SRCS) -o $(LRELEASE)

%: test/%_test.c
	$(CC) $(CFLAGS) $(DEBUG) $^ $(LINKED) -o a.out

clean:
	rm *.out *.so
