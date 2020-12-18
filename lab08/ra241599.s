.text
    .align 1
    .global _start

_start:    
    li a0, 856
    li a1, 1
    li a2 , -700
    li a7, 2100
    ecall

    li a0, 4700
    li a1, 1
    li a2 , 0
    li a7, 2100
    ecall


    li a0, 0
    li a7, 93
    ecall