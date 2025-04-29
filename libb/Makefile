.PHONY: all libb clean

ASFLAGS =

all: libb

debug: ASFLAGS += --gstabs
debug: libb

libb: brt1.o libb1.o

%.o: %.S
	cpp -nostdinc -x assembler-with-cpp -o $<.tmp $<
	as $(ASFLAGS) -o $@ $<.tmp

clean:
	find . -iname "*.o" -delete
	find . -iname "*.tmp" -delete
