dummy() ;

add(a,b) {
    return(a+b);
}

add10(a,b,c,d,e,f,g,h,i,j) {
    return(a+b+c+d+e+f+g+h+i+j);
}

main() {
    extrn putchar, printn;
    extrn add, add10, dummy;
    auto c;

    dummy();
    c = add('a', 1);
    putchar(c);
    putchar(add('a', 2));
    putchar('*n');
    printn(add10(1,2,3,4,5,6,7,8,9,10), 10);
    putchar('*n');
}
