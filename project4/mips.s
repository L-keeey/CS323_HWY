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
  move $a0, $t6
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  j label2
label1:
  move $t7, $a0
  li $s0, 1
  sub $s1, $t7, $s0
  move $s2, $a1
  move $s3, $a3
  move $s4, $a2
  addi $sp,$sp,-84
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
  sw $a0, 64($sp)
  sw $a1, 68($sp)
  sw $a2, 72($sp)
  sw $a3, 76($sp)
  sw $ra,80($sp)
  move $a0, $s4
  move $a1, $s3
  move $a2, $s2
  move $a3, $s1
  jal hanoi
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
  lw $a0, 64($sp)
  lw $a1, 68($sp)
  lw $a2, 72($sp)
  lw $a3, 76($sp)
  lw $ra,80($sp)
  addi $sp,$sp,84
  move $s5,$v0
  move $s6, $a1
  li $s7, 10000
  lw $t0, 0($sp)
  mul $t0, $s6, $s7
  lw $t1, 0($sp)
  move $t1, $a3
  lw $t2, 0($sp)
  add $t2, $t0, $t1
  move $a0, $t2
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  lw $t0, 0($sp)
  move $t0, $a0
  lw $t1, 0($sp)
  li $t1, 1
  lw $t2, 0($sp)
  sub $t2, $t0, $t1
  lw $t0, 0($sp)
  move $t0, $a2
  lw $t0, 0($sp)
  move $t0, $a1
  lw $t0, 0($sp)
  move $t0, $a3
  addi $sp,$sp,-84
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
  sw $a0, 64($sp)
  sw $a1, 68($sp)
  sw $a2, 72($sp)
  sw $a3, 76($sp)
  sw $ra,80($sp)
  move $a0, $t0
  move $a1, $t0
  move $a2, $t0
  move $a3, $t2
  jal hanoi
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
  lw $a0, 64($sp)
  lw $a1, 68($sp)
  lw $a2, 72($sp)
  lw $a3, 76($sp)
  lw $ra,80($sp)
  addi $sp,$sp,84
  lw $t0, 0($sp)
  move $t0,$v0
label2:
  lw $t0, 0($sp)
  li $t0, 0
  move $v0, $t0
  jr $ra
main:
  lw $t1, 0($sp)
  li $t1, 3
  lw $t2, 0($sp)
  move $t2, $t1
  lw $t1, 0($sp)
  move $t1, $t2
  lw $t1, 0($sp)
  li $t1, 1
  lw $t1, 0($sp)
  li $t1, 2
  lw $t1, 0($sp)
  li $t1, 3
  addi $sp,$sp,-84
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
  sw $a0, 64($sp)
  sw $a1, 68($sp)
  sw $a2, 72($sp)
  sw $a3, 76($sp)
  sw $ra,80($sp)
  move $a0, $t1
  move $a1, $t1
  move $a2, $t1
  move $a3, $t1
  jal hanoi
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
  lw $a0, 64($sp)
  lw $a1, 68($sp)
  lw $a2, 72($sp)
  lw $a3, 76($sp)
  lw $ra,80($sp)
  addi $sp,$sp,84
  lw $t1, 0($sp)
  move $t1,$v0
  lw $t1, 0($sp)
  li $t1, 0
  move $v0, $t1
  jr $ra
