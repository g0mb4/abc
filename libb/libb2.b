/* 
  The following function will print a non-negative number, n, to
  the base b, where 2<=b<=10,  This routine uses the fact that
  in the ANSCII character set, the digits O to 9 have sequential
  code values.
*/
printn(n,b) {
	extrn putchar;
	auto a;
	auto c;

	if(a=n/b) /* assignment, not test for equality */
		printn(a, b); /* recursive */

	putchar(n%b + '0');
}

/* 
  The character char is written on the standard output file.
*/
putchar(char) {
    extrn putc;
    auto i 0, b;

    while (i < 8) {
        b = (char >> (i++ * 8)) & 255;
        if (!b)
            return;

        putc(b);
    }
}
