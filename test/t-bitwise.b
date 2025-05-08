main() {
    extrn printn, putchar;
    auto a 10, b 6;

    /* 2 */
    printn(a&b, 10);
    putchar('*n');
    /* 14 */
    printn(a|b, 10);
    putchar('*n');

    /* 8 */
    printn(1<<3, 10);
    putchar('*n');
    /* 8 */
    printn(16>>1, 10);
    putchar('*n');
}
