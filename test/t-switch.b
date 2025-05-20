main() {
    extrn putchar;
    auto i 0;

    while (i < 4) {
        switch i {
        case 3:
            putchar('3*n');
        case 2:
            putchar('2*n');
        case 1:
            putchar('1*n');
        }
        i++;
    }

    i = 0;
    while (i < 4) {
        switch i {
        case 0:
        case 2:
            putchar('e*n');
            goto end;
        case 1:
        case 3:
            putchar('o*n');
        }
end:
        i++;
    }
}
