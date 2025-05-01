/* The following function will print a non-negative number, n, to
  the base b, where 2<=b<=10,  This routine uses the fact that
  in the ANSCII character set, the digits O to 9 have sequential
  code values.  */

printn(n,b) {
	extrn putchar;
	auto a;
	auto c;

	if(a=n/b)
		printn(a, b);

    /* TODO: original code was: 
        putchar(n%b + '0');
    */
	c = n%b;
	c = c + '0';
	putchar(c);
}