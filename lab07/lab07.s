.text
    .align 1
    .global _start

_start:
    # x -> t0
    # y -> t1
    li a6, 0
    while:
        li a4, 32
        li a5, 10
        jal ler
        la s0, input
        lb a0, (s0)
        bge a5, a0, endwhile
        bge a4, a0, posx
        mul a6, a6, a5 
        addi a0, a0, -48
        add a6, a6, a0
        j while
    posx:
        mv t0, a6
        li a6, 0
        j while
    endwhile:
        mv t1, a6
        
    teste_ro:
    #pular P2
    jal ler
    jal ler
    jal ler

    #col -> t2
    #lin -> t3
    li a6, 0
    while2:
        li a4, 32
        li a5, 10
        jal ler
        la s0, input
        lb a0, (s0)
        bge a5, a0, endwhile2
        bge a4, a0, posx2
        mul a6, a6, a5 
        addi a0, a0, -48
        add a6, a6, a0
        j while2
    posx2:
        mv t2, a6
        li a6, 0
        j while2
    endwhile2:
        mv t3, a6
        #addi t3, t3, -1
    teste_ro2:
    #MAX -> t4
    li a6, 0
    while3:
        li a5, 10
        jal ler
        la s0, input
        lb a0, (s0)
        bge a5, a0, endwhile3
        mul a6, a6, a5 #mult por 10 
        addi a0, a0, -48
        add a6, a6, a0
        j while3
    endwhile3:
        mv t4, a6

    while4: # pula primeira linha
        li a5, 10
        jal ler
        la s0, input
        lb a0, (s0)
        bge a5, a0, endwhile4
        j while4
    endwhile4:


    li s5, 0 #coluna
    li s6 , 0 #linha
    li s7, 100 #constante 100
    li s8, 0 #anterior
    #s10 -> muro esquerdo
    li a6, 0
    li s4, 0 #muro esquerdo não achado

    #ler linhas 
    while_linhas:
        beq s6, t3 , fim_while_linhas
        while5:
            li a4, 32
            li a5, 10
            jal ler
            la s0, input
            lb a0, (s0)
            bge a4, a0, posx5  # espaco
            mul a6, a6, a5 #x10
            addi a0, a0, -48
            add a6, a6, a0
            j while5
        posx5:
            
            bge a6, s7, endif1  
            blt s8, s7 , endif1

            emuro_esq: #if(atual < 100 && anterior > 100)
                li s4, 1 # muro esquerdo achado
                mv s10, s5
                mv s3, s5
                j endif2

            endif1:
            
            blt a6, s7, endif2 
            bge s8, s7 , endif2 
            beqz s4, endif2 # se não passou pelo muro esquerdo

            emuro_dir: #if(atual >100 && anterior < 100)
                mv s11, s5
                while6: # pula  linha
                    li a5, 10
                    jal ler
                    la s0, input
                    lb a0, (s0)
                    bge a5, a0, endwhile6
                    j while6
                endwhile6:
                j endwhile5

            endif2:

            mv s8, a6
            li a6, 0
            addi s5, s5, 1 #coluna++
            j while5


        endwhile5:
            
            #--> contas
            sub s10, s11, s10 # s10 <- diff
            li s11, 2
            div s10, s10, s11
            add s10, s10, s3

            #--> verificar qual caminho ir
            beq s10, t0, continua
            bge s10, t0, anda_direita
            blt s10, t0, anda_esquerda
            anda_direita:
                addi t0, t0, 1
                j continua
            anda_esquerda:
                addi t0, t0, -1
                j continua
            continua:

            addi t1, t1, 1
            addi s6, s6, 1 #linha++
            bge s6, t3, fim_while_linhas
            #--> imprimir -> pos
            li a0, 1 # file descriptor = 1 (stdout)
            la a1, pos #  buffer
            li a2, 5 # size
            li a7, 64 # syscall write (64)
            ecall
            
            la s0, div1000
            li s11, 1000
            div s11, t0, s11
            addi s11, s11, 48
            sw s11, (s0)  
            li a0, 1 # file descriptor = 1 (stdout)
            la a1, div1000 #  buffer
            li a2, 1 # size
            li a7, 64 # syscall write (64)
            ecall

            la s0, div100
            li s11, 100
            div s11, t0, s11
            addi s11, s11, 48
            sw s11, (s0)  
            li a0, 1 # file descriptor = 1 (stdout)
            la a1, div100 #  buffer
            li a2, 1 # size
            li a7, 64 # syscall write (64)
            ecall

            la s0, div10
            li s11, 10
            div s11, t0, s11
            addi s11, s11, 48
            sw s11, (s0)  
            li a0, 1 # file descriptor = 1 (stdout)
            la a1, div10 #  buffer
            li a2, 1 # size
            li a7, 64 # syscall write (64)
            ecall

            la s0, rest_div_10
            li s11, 10
            rem s11, t0, s11
            addi s11, s11, 48
            sw s11, (s0)  
            li a0, 1 # file descriptor = 1 (stdout)
            la a1, rest_div_10 #  buffer
            li a2, 1 # size
            li a7, 64 # syscall write (64)
            ecall

            # --> print y

            li a0, 1 # file descriptor = 1 (stdout)
            la a1, espaco #  buffer
            li a2, 1 # size
            li a7, 64 # syscall write (64)
            ecall
            
            la s0, div1000y
            li s11, 1000
            div s11, t1, s11
            addi s11, s11, 48
            sw s11, (s0)  
            li a0, 1 # file descriptor = 1 (stdout)
            la a1, div1000y #  buffer
            li a2, 1 # size
            li a7, 64 # syscall write (64)
            ecall

            la s0, div100y
            li s11, 100
            div s11, t1, s11
            addi s11, s11, 48
            sw s11, (s0)  
            li a0, 1 # file descriptor = 1 (stdout)
            la a1, div100y #  buffer
            li a2, 1 # size
            li a7, 64 # syscall write (64)
            ecall

            la s0, div10y
            li s11, 10
            div s11, t1, s11
            addi s11, s11, 48
            sw s11, (s0)  
            li a0, 1 # file descriptor = 1 (stdout)
            la a1, div10y #  buffer
            li a2, 1 # size
            li a7, 64 # syscall write (64)
            ecall

            la s0, rest_div_10y
            li s11, 10
            rem s11, t1, s11
            addi s11, s11, 48
            sw s11, (s0)  
            li a0, 1 # file descriptor = 1 (stdout)
            la a1, rest_div_10y #  buffer
            li a2, 1 # size
            li a7, 64 # syscall write (64)
            ecall

            li a0, 1 # file descriptor = 1 (stdout)
            la a1, barraN #  buffer
            li a2, 1 # size
            li a7, 64 # syscall write (64)
            ecall

            li s4, 0
            
            li s5, 0 #coluna = 0
            li s8, 0 #anterior = 0
            li a6, 0
            j while_linhas
    fim_while_linhas:
    li a0, 0
    li a7, 93
    ecall
    ler:
        li a0, 0
        la a1, input
        li a2, 1
        li a7, 63
        ecall
        ret
.align  4
    .data
    input:  .skip 3
    barraN: .ascii "\n"
    line:  .word 0
    column: .word 0
    pos: .ascii "POS: "
    espaco: .ascii " "
    div1000: .word 0
    div100: .word 0
    div10: .word 0
    rest_div_10: .word 0
    div1000y: .word 0
    div100y: .word 0
    div10y: .word 0
    rest_div_10y: .word 0