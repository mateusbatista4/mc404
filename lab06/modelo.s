.text
  .align 1
  .globl _start

_start:  
  # Converte angulo inteiro para radianos e coloca em f0
  jal funcao_pegar_angulo
  # Utilizado para calcular a série
  jal funcao_calcula_serie
  # Transforma um PF para inteiro, onde a0 contem o sinal, a1 a parte inteira e a2 a parte fracionaria (truncada com 3 casas decimais)
  jal funcao_float_para_inteiro
  # Imprime o resultado de a0, a1 e a2 na tela
  jal funcao_imprime
  
  li a0, 0 # exit code
  li a7, 93 # syscall exit
  ecall
  
funcao_calcula_serie: // fazer
  addi sp, sp, -8
  sw ra, 0(sp)
  sw s0, 4(sp)
  addi s0, sp, 8

  # f4  =>  (-1)^n
  # f7  =>   fatorial
  # f6  =>  2n+1
  # f5  =>   x^(2n+1)
  # f8  => soma  
  
  li a4, 10     # a4   =>  cosnt 10
  li s1, 0
  fsw f0, 0(s1) # f1   =>  x
  li a5, 0      # a5   =>  n
  fcvt.s.w f8, a5

  inicio_laco_principal:
    
    #(-1)^n
    li a6, 2
    rem a7, a5 , a6
    bnez a7 , impar
    beqz a7 , par
    impar:
      li a6, -1
      j endif
    par:
      li a6, 1
      j endif
    endif:
      fcvt.s.w f4, a6 

    # 2n+1
    li a6, 2
    mul a6, a6, a5
    addi a6, a6, 1
    fcvt.s.w f6, a6
    mv t1, a6

    #fatorial    
    li a7, 0
    fcvt.s.w f6, a6 
    fcvt.s.w f7, a6 # 2n+1
    fcvt.s.w f8, a7 # 1
    laco_fatorial:      
      bge a7, a6, fim_fat
      fsub.s f6, f6, f8
      fmul.s f7, f6, f7
      addi a6, a6, -1  
      j laco_fatorial    
    fim_fat: 

    # x^(2n+1)
    flw f5, 0(s1) #  <------ exp
    flw f3, 0(s1)
    # t1 = 2n + 1   
    laco_exp:
      addi a7, a7, 1
      bge a7, t1 , fim_laco_exp      
      fmul.s f5, f3, f5
      j laco_exp
    fim_laco_exp:


    fdiv.s f4 ,f4, f7
    fmul.s f4, f4, f5
    fadd.s  f8, f8, f4
    addi a5, a5, 1

    bge a5, a4, fim_laco_principal
    j inicio_laco_principal

  fim_laco_principal:
  fsw f8, 4(s1)
  flw f0, 4(s1)
  checar:
  lw ra, 0(sp)
  lw s0, 4(sp)
  addi sp, sp, 8
  jr ra


funcao_imprime: // fazer
  addi sp, sp, -8
  sw ra, 0(sp)
  sw s0, 4(sp)
  addi s0, sp, 8
  
  # Neste ponto os registradores contem:
  #   a0 -> valor 0 se f0 for negativo e !=0 caso contratio
  #   a1 -> Parte inteira de f0
  #   a2 -> Parte fracionaria de f0 (truncada com 3 casas decimais, i.e. 0 a 999)
  # **************************************
  # ** INSIRA AQUI SEU CÓDIGO PARA IMPRESSÃO **
  # **************************************
  
  lw ra, 0(sp)
  lw s0, 4(sp)
  addi sp, sp, 8
  jr ra

  
funcao_pegar_angulo:
  addi sp, sp, -8
  sw ra, 0(sp)
  sw s0, 4(sp) 
  addi s0, sp, 8
  
  # load angle value to a0
  lw a0, angle
  # convert angle to float and put in f0
  fcvt.s.w f0, a0
  # load pi address to a0
  la a0, .float_pi
  # load float_pi value (from a0 address) into f1
  flw f1, 0(a0)
  # load value 180 into a0
  li a0, 180
  # convert 180 to float and put in f2
  fcvt.s.w f2, a0

  # f0 -> angle, f1 -> pi, f2 -> 180
  # Now, put angle in radians (angle*pi/180)
  # f0 = angle * pi
  fmul.s f0, f0, f1
  # f0 = f0 / 180
  fdiv.s f0, f0, f2
  
  lw ra, 0(sp)
  lw s0, 4(sp)
  addi sp, sp, 8
  jr ra
  
funcao_float_para_inteiro:
  addi sp, sp, -8
  sw ra, 0(sp)
  sw s0, 4(sp)
  addi s0, sp, 8
  
  # Get signal
  li a0, 0
  fcvt.s.w f1, a0
  flt.s a0, f0, f1
  
  # Drop float signal
  fabs.s f0, f0
  
  # Truncate integer part
  fcvt.s.w f1, a0
  fadd.s f1, f1, f0
  jal funcao_truncar_float
  fcvt.w.s a1, f0
  
  # Truncate float part with 3 decimal places
  fsub.s f1, f1, f0
  li a3, 1000
  fcvt.s.w f2, a3
  fmul.s f0, f1, f2
  jal funcao_truncar_float
  fcvt.w.s a2, f0
  li a3, 1000
  rem a2, a2, a3
  
  lw ra, 0(sp)
  lw s0, 4(sp)
  addi sp, sp, 8
  jr ra
  
funcao_truncar_float:
  addi sp, sp, -8
  sw ra, 0(sp)
  sw s0, 4(sp)
  addi s0, sp, 8
  
  fmv.x.w a5, f0
  li a3, 22
  srai a4, a5,0x17
  andi a4, a4, 255
  addi a4, a4, -127
  addi a2, a5, 0
  blt a3, a4, .funcao_truncar_float_continue
  lui a5, 0x80000
  and a5, a5, a2
  bltz a4, .funcao_truncar_float_continue
  lui a5, 0x800
  addi a5, a5, -1
  sra a5, a5, a4
  not a5, a5
  and a5, a5, a2
.funcao_truncar_float_continue:
  fmv.w.x f0, a5
  
  lw ra, 0(sp)
  lw s0, 4(sp)
  addi sp, sp, 8
  jr ra
  
  
# Additional data variables
.align  4
.data
  angle:
    .word 45
  .float_pi:
    .word 0x40490fdb

