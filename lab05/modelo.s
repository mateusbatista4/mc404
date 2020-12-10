.globl _start

_start:
    li a0, 1 # file descriptor = 1 (stdout)
    la a1, string #  buffer
    li a2, 18 # size
    li a7, 64 # syscall write (64)
    ecall

    li a0, 0 # exit code
    li a7, 93 # syscall exit
    ecall

string:  .asciz "Mateus - ra241599\n"  #coloca a string na memoria
