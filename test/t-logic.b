main() {
    extrn putchar;
    auto a 20;
    auto b 0;
   
    if (0 < 1)
        putchar('1');
    else
        putchar('0');

    if (0 > 1)
        putchar('1');
    else
        putchar('0');

    if (1 <= 1)
        putchar('1');
    else
        putchar('0');

    if (1 >= 1)
        putchar('1');
    else
        putchar('0');

    if (1 == 1)
        putchar('1');
    else
        putchar('0');

    if (1 != 1)
        putchar('1');
    else
        putchar('0');

    if (a)
        putchar('1');
    else
        putchar('0');

    if (!a)
        putchar('1');
    else
        putchar('0');

    if (b)
        putchar('1');
    else
        putchar('0');

    if (!b)
        putchar('1');
    else
        putchar('0');

    putchar('*n');
}
