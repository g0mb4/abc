main() {
    extrn putchar;
    auto b 'b';
    auto one 1;
    auto c;

    putchar('1'+1);
    putchar('*n');

    putchar(b+one);
    putchar(b-one);
    putchar('*n');
    
    c = b + one;
    putchar(c);

    c = b - one;
    putchar(c);
    putchar('*n');

    c = '1' * 2;
    putchar(c);

    c = 'b' / 2;
    putchar(c);
    putchar('*n');
}
