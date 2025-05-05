main() {
    extrn printn, putchar;
    auto cnt 5;

    while (cnt) {
        printn(cnt, 10);
        cnt = cnt - 1;
    }
    putchar('*n');
}
