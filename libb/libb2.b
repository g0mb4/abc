/* 
  The character char is written on the standard output file.
*/
putchar(char) {
  extrn write;

  if (char >= (1 << 56))
    write(1, &char, 8);
  else if (char >= (1 << 48))
    write(1, &char, 7);
  else if (char >= (1 << 40))
    write(1, &char, 6);
  else if (char >= (1 << 32))
    write(1, &char, 5);
  else if (char >= (1 << 24))
    write(1, &char, 4);
  else if (char >= (1 << 16))
    write(1, &char, 3);
  else if (char >= (1 << 8))
    write(1, &char, 2);
  else
    write(1, &char, 1);
}

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
