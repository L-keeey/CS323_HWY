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
  move $a0, $s1
  move $a1, $s2
  move $a2, $s3
  move $a3, $s4
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
  li $fp, 268510996
  lw $t0, 4($fp)
  mul $t0, $s6, $s7
  li $fp, 268511000
  lw $t1, 8($fp)
  move $t1, $a3
  li $fp, 268511004
  lw $t2, 12($fp)
  add $t2, $t0, $t1
  addi $sp, $sp, -8
  sw $a0, 0($sp)
  move $a0, $t2
  sw $ra, 4($sp)
  jal write
  lw $a0, 0($sp)
  lw $ra, 4($sp)
  addi $sp, $sp, 8
  li $fp, 268511008
  lw $t3, 16($fp)
  move $t3, $a0
  li $fp, 268511012
  lw $t4, 20($fp)
  li $t4, 1
  li $fp, 268511016
  lw $t5, 24($fp)
  sub $t5, $t3, $t4
  li $fp, 268511020
  lw $t6, 28($fp)
  move $t6, $a2
  li $fp, 268511024
  lw $s1, 32($fp)
  move $s1, $a1
  li $fp, 268511028
  lw $t7, 36($fp)
  move $t7, $a3
  addi $sp,$sp,-84
  sw $a0, 0($sp)
  sw $a1, 4($sp)
  sw $a2, 8($sp)
  sw $a3, 12($sp)
  move $a0, $t5
  move $a1, $t6
  move $a2, $s1
  move $a3, $t7
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
  li $fp, 268511032
  lw $s0, 40($fp)
  move $s0,$v0
label2:
  li $fp, 268511036
  lw $s2, 44($fp)
  li $s2, 0
  move $v0, $s2
  jr $ra
main:
  li $fp, 268511040
  lw $s3, 48($fp)
  li $s3, 3
  li $fp, 268511044
  lw $s4, 52($fp)
  move $s4, $s3
  li $fp, 268511048
  lw $s5, 56($fp)
  move $s5, $s4
  li $fp, 268511052
  lw $s6, 60($fp)
  li $s6, 1
  li $fp, 268511056
  lw $s7, 64($fp)
  li $s7, 2
  li $fp, 268511060
  lw $t0, 68($fp)
  li $t0, 3
  addi $sp,$sp,-84
  sw $a0, 0($sp)
  sw $a1, 4($sp)
  sw $a2, 8($sp)
  sw $a3, 12($sp)
  move $a0, $s5
  move $a1, $s6
  move $a2, $s7
  move $a3, $t0
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
  li $fp, 268511064
  lw $t1, 72($fp)
  move $t1,$v0
  li $fp, 268511068
  lw $t2, 76($fp)
  li $t2, 0
  move $v0, $t2
  jr $ra
