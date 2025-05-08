main() {
    extrn printn, putchar;
    auto a 10, b 6;

    printn(a&b, 10);
    putchar('*n');
    printn(a|b, 10);
    putchar('*n');
}
