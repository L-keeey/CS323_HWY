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
hanoi:
  move $t0, $a0
  li $t1, 1
  beq $t0, $t1, label0
  j label1
label0:
  move $t2, $a1
  li $t3, 10000
  mul $t4, $t2, $t3
  move $t5, $a3
  add $t6, $t4, $t5
  addi $sp, $sp, -8
  sw $a0, 0($sp)
  move $a0, $t6
  sw $ra, 4($sp)
  jal write
  lw $a0, 0($sp)
  lw $ra, 4($sp)
  addi $sp, $sp, 8
  j label2
label1:
  move $t7, $a0
  li $s0, 1
  sub $s1, $t7, $s0
  move $s2, $a1
  move $s3, $a3
  move $s4, $a2
  addi $sp,$sp,-72
  sw $a0, 0($sp)
  sw $a1, 4($sp)
  sw $a2, 8($sp)
  sw $a3, 12($sp)
  move $a0, $s4
  move $a1, $s3
  move $a2, $s2
  move $a3, $s1
  sw $t0, 16($sp)
  sw $t1, 20($sp)
  sw $t2, 24($sp)
  sw $t3, 28($sp)
  sw $t4, 32($sp)
  sw $t5, 36($sp)
  sw $t6, 40($sp)
  sw $t7, 44($sp)
  sw $s0, 48($sp)
  sw $s1, 52($sp)
  sw $s2, 56($sp)
  sw $s3, 60($sp)
  sw $s4, 64($sp)
  sw $ra,68($sp)
  jal hanoi
  lw $a0, 0($sp)
  lw $a1, 4($sp)
  lw $a2, 8($sp)
  lw $a3, 12($sp)
  lw $t0, 16($sp)
  lw $t1, 20($sp)
  lw $t2, 24($sp)
  lw $t3, 28($sp)
  lw $t4, 32($sp)
  lw $t5, 36($sp)
  lw $t6, 40($sp)
  lw $t7, 44($sp)
  lw $s0, 48($sp)
  lw $s1, 52($sp)
  lw $s2, 56($sp)
  lw $s3, 60($sp)
  lw $s4, 64($sp)
  lw $ra,68($sp)
  addi $sp,$sp,72
  move $s5,$v0
  move $s6, $a1
  li $s7, 10000
  sw $t0, 4($sp)
  mul $t0, $s6, $s7
  sw $t1, 8($sp)
  move $t1, $a3
  sw $t2, 12($sp)
  add $t2, $t0, $t1
  addi $sp, $sp, -8
  sw $a0, 0($sp)
  move $a0, $t2
  sw $ra, 4($sp)
  jal write
  lw $a0, 0($sp)
  lw $ra, 4($sp)
  addi $sp, $sp, 8
  sw $t3, 16($sp)
  move $t3, $a0
  sw $t4, 20($sp)
  li $t4, 1
  sw $t5, 24($sp)
  sub $t5, $t3, $t4
  sw $t6, 28($sp)
  move $t6, $a2
  sw $s1, 32($sp)
  move $s1, $a1
  sw $t7, 36($sp)
  move $t7, $a3
  addi $sp,$sp,-84
  sw $a0, 0($sp)
  sw $a1, 4($sp)
  sw $a2, 8($sp)
  sw $a3, 12($sp)
  move $a0, $t7
  move $a1, $s1
  move $a2, $t6
  move $a3, $t5
  sw $t0, 16($sp)
  sw $t1, 20($sp)
  sw $t2, 24($sp)
  sw $t3, 28($sp)
  sw $t4, 32($sp)
  sw $t5, 36($sp)
  sw $t6, 40($sp)
  sw $t7, 44($sp)
  sw $s0, 48($sp)
  sw $s1, 52($sp)
  sw $s2, 56($sp)
  sw $s3, 60($sp)
  sw $s4, 64($sp)
  sw $s5, 68($sp)
  sw $s6, 72($sp)
  sw $s7, 76($sp)
  sw $ra,80($sp)
  jal hanoi
  lw $a0, 0($sp)
  lw $a1, 4($sp)
  lw $a2, 8($sp)
  lw $a3, 12($sp)
  lw $t0, 16($sp)
  lw $t1, 20($sp)
  lw $t2, 24($sp)
  lw $t3, 28($sp)
  lw $t4, 32($sp)
  lw $t5, 36($sp)
  lw $t6, 40($sp)
  lw $t7, 44($sp)
  lw $s0, 48($sp)
  lw $s1, 52($sp)
  lw $s2, 56($sp)
  lw $s3, 60($sp)
  lw $s4, 64($sp)
  lw $s5, 68($sp)
  lw $s6, 72($sp)
  lw $s7, 76($sp)
  lw $ra,80($sp)
  addi $sp,$sp,84
  sw $s0, 40($sp)
  move $s0,$v0
label2:
  sw $s2, 44($sp)
  li $s2, 0
  move $v0, $s2
  jr $ra
main:
  sw $s3, 48($sp)
  li $s3, 3
  sw $s4, 52($sp)
  move $s4, $s3
  sw $s5, 56($sp)
  move $s5, $s4
  sw $s6, 60($sp)
  li $s6, 1
  sw $s7, 64($sp)
  li $s7, 2
  sw $t0, 68($sp)
  li $t0, 3
  addi $sp,$sp,-84
  sw $a0, 0($sp)
  sw $a1, 4($sp)
  sw $a2, 8($sp)
  sw $a3, 12($sp)
  move $a0, $t0
  move $a1, $s7
  move $a2, $s6
  move $a3, $s5
  sw $t0, 16($sp)
  sw $t1, 20($sp)
  sw $t2, 24($sp)
  sw $t3, 28($sp)
  sw $t4, 32($sp)
  sw $t5, 36($sp)
  sw $t6, 40($sp)
  sw $t7, 44($sp)
  sw $s0, 48($sp)
  sw $s1, 52($sp)
  sw $s2, 56($sp)
  sw $s3, 60($sp)
  sw $s4, 64($sp)
  sw $s5, 68($sp)
  sw $s6, 72($sp)
  sw $s7, 76($sp)
  sw $ra,80($sp)
  jal hanoi
  lw $a0, 0($sp)
  lw $a1, 4($sp)
  lw $a2, 8($sp)
  lw $a3, 12($sp)
  lw $t0, 16($sp)
  lw $t1, 20($sp)
  lw $t2, 24($sp)
  lw $t3, 28($sp)
  lw $t4, 32($sp)
  lw $t5, 36($sp)
  lw $t6, 40($sp)
  lw $t7, 44($sp)
  lw $s0, 48($sp)
  lw $s1, 52($sp)
  lw $s2, 56($sp)
  lw $s3, 60($sp)
  lw $s4, 64($sp)
  lw $s5, 68($sp)
  lw $s6, 72($sp)
  lw $s7, 76($sp)
  lw $ra,80($sp)
  addi $sp,$sp,84
  sw $t1, 72($sp)
  move $t1,$v0
  sw $t2, 76($sp)
  li $t2, 0
  move $v0, $t2
  jr $ra
