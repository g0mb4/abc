main() {
    extrn putchar;
    auto i 65;

    putchar(++i);
    putchar(i++);

    putchar(--i);
    putchar(i--);

    putchar(i);

    putchar('*n');
}
