.PHONY: all libb clean

all: libb

libb: brt1.o libb1.o

%.o: %.S
	cpp -nostdinc -x assembler-with-cpp -o $<.tmp $<
	as -o $@ $<.tmp

clean:
	find . -iname "*.o" -delete
	find . -iname "*.tmp" -delete
