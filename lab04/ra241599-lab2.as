.org 0x000
    LOAD MQ,M(entrada)
    MUL M(g)
    LOAD MQ 
    STOR M(y)
    DIV M(const_2)
    LOAD MQ
    STOR M(k)
laco:
    LOAD M(y)   
    DIV M(k)
    LOAD MQ
    ADD M(k) 
    DIV M(const_2) 
    LOAD MQ 
    STOR M(k)
    LOAD M(n)
    SUB M(const_1)
    STOR M(n)
    JUMP+ M(laco,20:39) 
    LOAD M(k)  
    JUMP M(0x400)

.org 0x110
entrada:

.org 0x111
g: .word 0000000010

.org 0x112
const_2: .word 0000000002

.org 0x113 
y:

.org 0x114
k:

.org 0x115
n: .word 0000000010

.org 0x116
const_1: .word 0000000001
