main() {
    extrn putchar;
    auto c;

    c = 1 ? '1' : '0';
    putchar(c);

    c = 0 ? '1' : '0';
    putchar(c);

    putchar('*n');
}