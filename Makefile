.PHONY: b libb clean

all: b libb

b:
	make -C b

libb:
	make -C libb

clean:
	make -C b clean
	make -C libb clean


