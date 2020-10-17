

.org 0x065
soma: .word 0000000000
.org 0x066
const_1: .word 0000000001
.org 0x067
i: .word 0000000000

.org 0x3FE
end_v2:

.org 0x3FF
end_v1: 

.org 0x3FD 
len:

.org 0x000
inicio:

    LOAD M(end_v2)
    ADD M(i)
    STA M(003,8:19)  
    LOAD M(end_v1)
    ADD M(i) 
    STA M(003,28:39)
oper: 
    LOAD MQ,M(0000) 
    MUL M(0000)
    LOAD MQ 
    ADD M(soma)
    STOR M(soma) 
incremi: 
    LOAD M(i)
    ADD M(const_1) 
    STOR M(i)
condicional: 
    LOAD M(len) 
    SUB M(const_1)
    SUB M(i)
    
    JUMP+ M(inicio, 0:19)
    LOAD M(soma) 
    JUMP M(0x400)