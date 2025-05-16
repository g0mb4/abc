main() {
    extrn printf;
    auto a 1, b 2;

    printf("Hello*n");
    printf("d (9) = %d*n", 9);
    printf("o (9) = %o*n", 9);
    printf("c (a) = %c*n", 'a');
    printf("s (Hello) = %s*n", "Hello");
    printf("%% = %%*n");

    printf("%d + %d = %d*n", a, b, a+b);
}
