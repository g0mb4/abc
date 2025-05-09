main() {
    extrn printn, putchar;
    auto a 20;
    auto b 0;
   
    printn(0 < 1, 10);   /* 1 */
    printn(0 > 1, 10);   /* 0 */
    printn(1 <= 1, 10);  /* 1 */
    printn(1 >= 1, 10);  /* 1 */
    printn(1 == 1, 10);  /* 1 */
    printn(1 != 1, 10);  /* 0 */

    /* 1 */
    if (a)
        putchar('1');
    else
        putchar('0');

    /* 0 */
    if (!a)
        putchar('1');
    else
        putchar('0');

    /* 0 */
    if (b)
        putchar('1');
    else
        putchar('0');

    /* 1 */
    if (!b)
        putchar('1');
    else
        putchar('0');

    putchar('*n');
}
