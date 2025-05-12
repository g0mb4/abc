puts(s) {
    extrn char, putchar;
    auto c, i 0;

    while((c=char(s,i++)) != '*e')
        putchar(c);

    putchar('*n');
}

main() {
    extrn puts;
    auto s, ss "from B";
   
    s = "World";

    puts("Hello");
    puts(s);
    puts(ss);
}
