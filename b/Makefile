CC := gcc
CFLAGS := -std=gnu99 -Wall -Wextra

.PHONY: all b clean

all: b

b: main.o ast.o codegen.o b.tab.o lex.yy.o
	$(CC) -o $@ $^

b.tab.c b.tab.y: b.y
	bison --debug -d $<

lex.yy.c: b.l b.tab.h
	flex --debug $<

clean:
	find . -type f -name "b" -delete
	find . -type f -name "*.tab.c" -delete
	find . -type f -name "*.tab.h" -delete
	find . -type f -name "lex.yy.c" -delete
	find . -type f -name "*.o" -delete
