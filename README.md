# B

B compiler targeting `x86_64-linux` based on [Ken Thompson's manual](doc/kbman.html).

## Quickstart

```bash
make
b/b examples/hw.b
as -o b.out.o b.out.s
ld -o hw libb/brt1.o b.out.o libb/libb1.o
./hw
```
