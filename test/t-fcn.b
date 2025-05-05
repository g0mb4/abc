dummy() ;

add(a,b) {
    return(a + b);
}

main() {
    extrn putchar, add, dummy;
    auto c;

    dummy();
    c = add('a', 1);
    putchar(c);
    putchar(add('a', 2));
    putchar('*n');
}
