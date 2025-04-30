dummy() ;

add(a,b) {
    return(a + b);
}

main() {
    extrn putchar;
    extrn add;
    extrn dummy;
    auto c;

    dummy();
    c = add('a', 1);
    putchar(c);
    putchar(add('a', 2));
    putchar('*n');
}
