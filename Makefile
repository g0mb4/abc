.PHONY: b libb clean check

DEBUG =

all: b libb

debug: DEBUG += debug
debug: all

b:
	make -C b $(DEBUG)

libb:
	make -C libb $(DEBUG)

clean:
	make -C b clean
	make -C libb clean

check: all
	cd test; bash test.sh
