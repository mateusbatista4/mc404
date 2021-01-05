.globl set_motor
.globl set_handbreak
.globl read_sensors
.globl read_sensor_distance
.globl get_time
.globl get_position
.globl get_rotation

# Implemente aqui as funções da API_CAR.
# As funções devem checar os parametros e fazer as chamadas de sistema que serão
#   tratadas na camada SoCa

set_motor:
    addi sp, sp, -4
    sw ra, 0(sp)
    li a7, 10
    ecall
    lw ra, 0(sp)
    addi sp, sp, 4
    ret

set_handbreak:
    addi sp, sp, -4
    sw ra, 0(sp)
    li a7, 11
    ecall
    lw ra, 0(sp)
    addi sp, sp, 4
    ret

read_sensors:
    addi sp, sp, -4
    sw ra, 0(sp)
    li a7, 12
    ecall
    lw ra, 0(sp)
    addi sp, sp, 4
    ret

read_sensor_distance:
    addi sp, sp, -4
    sw ra, 0(sp)
    li a7, 13
    ecall
    lw ra, 0(sp)
    addi sp, sp, 4
    ret

get_time:
    addi sp, sp, -4
    sw ra, 0(sp)
    li a7, 14
    ecall
    lw ra, 0(sp)
    addi sp, sp, 4
    ret


get_position:
    addi sp, sp, -4
    sw ra, 0(sp)
    li a7, 15
    ecall
    lw ra, 0(sp)
    addi sp, sp, 4
    ret

get_rotation:
    addi sp, sp, -4
    sw ra, 0(sp)
    li a7, 16
    ecall
    lw ra, 0(sp)
    addi sp, sp, 4
    ret

