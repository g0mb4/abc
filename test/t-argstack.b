arg(a,b,c){
    extrn printn;
    auto p;

    p = &b;
    printn(*p, 10);

    p =+ 8;  /* TODO: pointer arithmetic */
    printn(*p, 10);
}

main() {
    extrn arg, putchar;

    arg(1, 2, 3);
    putchar('*n');
}
