main() {
   extrn printf;
   auto p, a 1, b 2, c 3, d 4, e 5;
   auto r;

   p = &a;

   r = *p;
   p++;

   printf("%d*n", r);
   printf("%d*n", *p++);
   printf("%d*n", *++p);
}

