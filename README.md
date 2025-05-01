# ABC

A B Compiler targeting `x86_64-linux` based on [Ken Thompson's manual](doc/kbman.html).

> [!WARNING]
> Compiler is not fully implemented yet, maybe it will never will be.

## Quickstart

```bash
make
b/b examples/hw.b
as -o b.out.o b.out.s
ld -o hw libb/brt1.o b.out.o libb/libb.a
./hw
```

## Tests

```bash
make check
```

If you want to add a new test, modify *test/test.sh*s `tests` list, then
`./test.sh gen`.
