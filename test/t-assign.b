main() {
    extrn printn, putchar;
    auto a, b;

    /* 2 */
    a = 2;
    printn(a, 10);
    putchar('*n');

    /* 33 */
    a = b = 3;
    printn(a, 10);
    printn(b, 10);
    putchar('*n');

    /* 5 */
    a =+ 2;
    printn(a, 10);
    putchar('*n');

    /* 4 */
    a =- 1;
    printn(a, 10);
    putchar('*n');

    /* 12 */
    a =* 3;
    printn(a, 10);
    putchar('*n');

    /* 3 */
    a =/ 4;
    printn(a, 10);
    putchar('*n');

    /* 1 */
    b = 21;
    b =% 4;
    printn(b, 10);
    putchar('*n');
}
