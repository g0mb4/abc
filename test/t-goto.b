main() {
    extrn putchar, printn;
    auto i 0;

loop:
    if (i < 5) {
        printn(i++, 10);
        putchar('*n');

        goto loop;
    }
}
