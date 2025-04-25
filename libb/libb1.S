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
    .global write
write:
    movq SYS_write, %rax
    syscall

    .global exit
exit:
    movq SYS_exit, %rax
    movq $0, %rdi
    syscall
