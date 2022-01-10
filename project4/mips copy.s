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
  beq $a0, $t8, label1 //if v1=1 goto label 1
  j label2
label1:
  move $v0, $a0 // return v1
  jr $ra
label2:
  addi $t0, $a0, -1 //t1=v1-1 t1=t0
  addi $sp,$sp,-68
  sw $t0, 0($sp)
  sw $t1, 4($sp)
  sw $t2, 8($sp)
  sw $t3, 12($sp)
  sw $t4, 16($sp)
  sw $t5, 20($sp)
  sw $t6, 24($sp)
  sw $t7, 28($sp)
  sw $s0, 32($sp)
  sw $s1, 36($sp)
  sw $s2, 40($sp)
  sw $s3, 44($sp)
  sw $s4, 48($sp)
  sw $s5, 52($sp)
  sw $s6, 56($sp)
  sw $s7, 60($sp)
  sw $ra,64($sp)
  addi $sp,$sp,-16
  sw $a0, 0($sp)
  sw $a1, 4($sp)
  sw $a2, 8($sp)
  sw $a3, 12($sp)
  move $a0, $t0 //arg t1
  jal fact 
  lw $t0, 0($sp)
  lw $t1, 4($sp)
  lw $t2, 8($sp)
  lw $t3, 12($sp)
  lw $t4, 16($sp)
  lw $t5, 20($sp)
  lw $t6, 24($sp)
  lw $t7, 28($sp)
  lw $s0, 32($sp)
  lw $s1, 36($sp)
  lw $s2, 40($sp)
  lw $s3, 44($sp)
  lw $s4, 48($sp)
  lw $s5, 52($sp)
  lw $s6, 56($sp)
  lw $s7, 60($sp)
  lw $ra,64($sp)
  addi $sp,$sp,68
  lw $a0, 0($sp)
  lw $a1, 4($sp)
  lw $a2, 8($sp)
  lw $a3, 12($sp)
  addi $sp,$sp,16
  move $t1,$v0 //t2= call fact t2=t1
  mul $t2, $a0, $t1 //t3=v1*t2  t3=t2
  move $v0, $t2 //return t3
  jr $ra
main:
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal read 
  lw $ra, 0($sp) 
  addi $sp, $sp, 4
  move $t3, $v0 //read t4  t4=t3
  move $t4, $t3 //v2=t4 v2=t4
  li $t9, 1
  bgt $t4, $t9, label3 //if v2>1 
  j label4
label3:
  addi $sp,$sp,-68
  sw $t0, 0($sp)
  sw $t1, 4($sp)
  sw $t2, 8($sp)
  sw $t3, 12($sp)
  sw $t4, 16($sp)
  sw $t5, 20($sp)
  sw $t6, 24($sp)
  sw $t7, 28($sp)
  sw $s0, 32($sp)
  sw $s1, 36($sp)
  sw $s2, 40($sp)
  sw $s3, 44($sp)
  sw $s4, 48($sp)
  sw $s5, 52($sp)
  sw $s6, 56($sp)
  sw $s7, 60($sp)
  sw $ra,64($sp)
  addi $sp,$sp,-16
  sw $a0, 0($sp)
  sw $a1, 4($sp)
  sw $a2, 8($sp)
  sw $a3, 12($sp)
  move $a0, $t4 //ARG v2
  jal fact // call fact
  lw $t0, 0($sp)
  lw $t1, 4($sp)
  lw $t2, 8($sp)
  lw $t3, 12($sp)
  lw $t4, 16($sp)
  lw $t5, 20($sp)
  lw $t6, 24($sp)
  lw $t7, 28($sp)
  lw $s0, 32($sp)
  lw $s1, 36($sp)
  lw $s2, 40($sp)
  lw $s3, 44($sp)
  lw $s4, 48($sp)
  lw $s5, 52($sp)
  lw $s6, 56($sp)
  lw $s7, 60($sp)
  lw $ra,64($sp)
  addi $sp,$sp,68
  lw $a0, 0($sp)
  lw $a1, 4($sp)
  lw $a2, 8($sp)
  lw $a3, 12($sp)
  addi $sp,$sp,16
  move $t5,$v0 // t5=call fact t5=t5
  move $t6, $t5 //v3=t5 v3=t6
  j label5
label4:
  li $t6, 1 //v3=1
label5:
  move $a0, $t6 // write v3
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4 
  li $t8, 0
  move $v0, $t8 //return 0
  jr $ra
