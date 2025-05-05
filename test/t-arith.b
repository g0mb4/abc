main() {
    extrn putchar, printn;
    auto a, b, c;

    a = 2;
    b = 3;

    /* 5 */
    c = b + a;
    printn(c, 10);
    putchar('*n');

    /* 1 */
    c = b - a;
    printn(c, 10);
    putchar('*n');

    /* 6 */
    c = b * a;
    printn(c, 10);
    putchar('*n');

    /* 3 */
    c = 6 / a;
    printn(c, 10);
    putchar('*n');
    
    /* 2 */
    c = 6 % 4;
    printn(c, 10);
    putchar('*n');

    /* 7 */
    printn(1+2*3, 10);
    putchar('*n');

    /* 9 */
    printn((1+2)*3, 10);
    putchar('*n');

    /* 9 */
    printn(3*(1+2), 10);
    putchar('*n');
}
