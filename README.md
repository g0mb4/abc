# ABC

A B Compiler targeting `x86_64-linux` based on [Ken Thompson's manual](doc/kbman.html).

> [!WARNING]
> Compiler is not fully implemented yet, maybe it will never will be.

The aim of this compiler is to compile the examples given in the manual, nothing more.

Working examples:
- [x] 9.1 (libb/libb2.b)
- [x] 9.2 (examples/e-2.b)
- [x] 9.3 (libb/libb2.b)

## Quickstart

```bash
make
b/b examples/hw.b
as -o b.out.o b.out.s
ld -o hw libb/brt1.o b.out.o libb/libb.a
./hw
```

## Debug

Run ```make debug``` then
```bash
b/b examples/hw.b
```
to get a **lot** of debug information.

## Tests

```bash
make check
```

If you want to add a new test, modify *test/test.sh*s `tests` list by adding the name of the test file without the extension, then
`./test.sh gen`.
