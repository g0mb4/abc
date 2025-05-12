/*
    Assembly part of libB
*/

/*
    x86_64 calling convention:

    function call parameters:
        %rdi, %rsi, %rdx, %rcx, %r8, %r9
    return value:
        %rax

    system call parameters:
        %rdi, %rsi, %rdx, %r10, %r8, %r9
    return value:
        %rax
*/

#define SYS_write   $1
#define SYS_exit    $60

    .text

/*
    registers already have their values set via regular function calls
*/

/*
    nwrite = write(file, buffer, count);

    Count bytes are written out of the vector buffer on the open
    file designated by file.  The actual number of bytes written
    are returned. A negative number returned indicates an error.
*/
    .global write
write:
    movq SYS_write, %rax
    syscall
    ret

/*
    exit();

    The current process is terminated.
*/
    .global exit
exit:
    movq SYS_exit, %rax
    movq $0, %rdi
    syscall
    ret

/*
    putc(c);

    The lower 8 bits of c is written on the standard output file.
*/
    .global putc
putc:
    pushq %rbp
    movq %rsp, %rbp
    subq $8, %rsp
    movq %rdi, -8(%rbp)
    movq $1, %rdi
    leaq -8(%rbp), %rsi
    movq $1, %rdx
    call write
    addq $8, %rsp
    popq %rbp
    ret

/*
    c = char(string, i);

    The i-th character of the string is returned.

    C:
    word char(word string, word i) {
        char *s = (char*)string;
        return (char)s[i];
    }
*/
    .global char
char:
    pushq   %rbp
    movq    %rsp, %rbp
    movq    %rdi, -24(%rbp)
    movq    %rsi, -32(%rbp)
    movq    -24(%rbp), %rax
    movq    %rax, -8(%rbp)
    movq    -32(%rbp), %rdx
    movq    -8(%rbp), %rax
    addq    %rdx, %rax
    movzbl  (%rax), %eax
    movsbq  %al, %rax
    popq    %rbp
    ret
