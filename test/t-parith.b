main() {
   extrn printf;
   auto p, a 1, b 2, c 3;

   p = &b;
   p++;

   printf("%d*n", *p);
}

