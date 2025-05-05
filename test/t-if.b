main() {
    extrn putchar;
    auto t 1, f 0;

    if (t)
        putchar('a');

    if (t) {
        putchar('b');
        putchar('b');
    } else {
        putchar('c');
        putchar('c');
    }

    putchar('*n');

    if (f)
        putchar('a');

    if (f) {
        putchar('b');
        putchar('b');
    } else {
        putchar('c');
        putchar('c');
    }

    putchar('*n');
}
