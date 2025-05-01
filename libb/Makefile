.PHONY: all libb clean

B := "../b/b"
ASFLAGS =

all: libb

debug: ASFLAGS += --gstabs
debug: libb

libb: brt1.o libb.a

libb.a: libb1.o libb2.o
	ar crs $@ $^

%.o: %.b
	$(B) $<
	as $(ASFLAGS) -o $@ b.out.s

%.o: %.S
	cpp -nostdinc -x assembler-with-cpp -o $<.tmp $<
	as $(ASFLAGS) -o $@ $<.tmp

clean:
	find . -iname "*.o" -delete
	find . -iname "b.out.s" -delete
	find . -iname "*.tmp" -delete
