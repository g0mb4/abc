main() {
    extrn putchar, printn;
    auto a;

    /* 14 */
    a = 2 + 3 * 4;
    printn(a, 10);
    putchar('*n');

    /* 14 */
    a = 3 * 4 + 2;
    printn(a, 10);
    putchar('*n');

    /* 14 */
    a = (3 * 4) + 2;
    printn(a, 10);
    putchar('*n');

    /* 20 */
    a = (2 + 3) * 4;
    printn(a, 10);
    putchar('*n');
}
