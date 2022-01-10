# SPL compiler generated assembly
.data
_prmpt: .asciiz "Enter an integer: "
_eol: .asciiz "\n"
.globl main
.text
read:
  li $v0, 4
  la $a0, _prmpt
  syscall
  li $v0, 5
  syscall
  jr $ra
write:
  li $v0, 1
  syscall
  li $v0, 4
  la $a0, _eol
  syscall
  move $v0, $0
  jr $ra
fact:
  li $t8, 1
  beq $a0, $t8, label1
  j label2
label1:
  move $v0, $a0
  jr $ra
label2:
  addi $t0, $a0, -1
  addi $sp,$sp,-8
  sw $a0, 0($sp)
  sw $ra, 4($sp)
  move $a0, $t0
  jal fact
  lw $a0, 0($sp)
  lw $ra, 4($sp)
  addi $sp,$sp,8
  move $t1,$v0
  mul $t2, $a0, $t1
  move $v0, $t2
  jr $ra
main:
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal read
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  move $t3, $v0
  move $t4, $t3
  li $t9, 1
  bgt $t4, $t9, label3
  j label4
label3:
  addi $sp,$sp,-8
  sw $a0, 0($sp)
  sw $ra, 4($sp)
  move $a0, $t4
  jal fact
  lw $a0, 0($sp)
  lw $ra, 4($sp)
  addi $sp,$sp,8
  move $t5,$v0
  move $t6, $t5
  j label5
label4:
  li $t6, 1
label5:
  move $a0, $t6
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  li $t8, 0
  move $v0, $t8
  jr $ra
