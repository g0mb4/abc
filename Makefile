.PHONY: b libb clean

DEBUG =

all: b libb

debug: DEBUG += debug
debug: all

b:
	make -C b $(DEBUG)

libb:
	make -C libb

clean:
	make -C b clean
	make -C libb clean


