CC := gcc
CFLAGS = -std=gnu99 -Wall -Wextra
LDFLAGS =
YFLAGS = 
YYFLAGS = 

.PHONY: all b clean debug

all: b

debug: CFLAGS += -ggdb -DDEBUG
debug: LDFLAGS += 
debug: YFLAGS += --debug
debug: YYFLAGS += --debug
debug: b

b: main.o ast.o codegen.o b.tab.o lex.yy.o helpers.o
	$(CC) -o $@ $^ $(LDFLAGS)

b.tab.c b.tab.y: b.y
	bison $(YFLAGS) -d $<

lex.yy.c: b.l b.tab.h
	flex $(YYFLAGS)  $<

clean:
	find . -type f -name "b" -delete
	find . -type f -name "*.tab.c" -delete
	find . -type f -name "*.tab.h" -delete
	find . -type f -name "lex.yy.c" -delete
	find . -type f -name "*.o" -delete
