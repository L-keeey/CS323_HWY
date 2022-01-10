#ifndef MIPS_H
#define MIPS_H

#include "tac.h"
#define FALSE 0
#define TRUE 1
typedef unsigned char bool;

typedef enum {
    zero, at, v0, v1, a0, a1, a2, a3,
    t0, t1, t2, t3, t4, t5, t6, t7,
    s0, s1, s2, s3, s4, s5, s6, s7,
    t8, t9, k0, k1, gp, sp, fp, ra, NUM_REGS
} Register;


struct RegDesc {    // the register descriptor
    const char *name;
    char var[8];
    bool dirty; // value updated but not stored
    /* add other fields as you need */
    int ttl;
    int time;
} regs[NUM_REGS];


struct VarDesc {    // the variable descriptor
    char var[8];
    Register reg;
    int offset; // the offset from stack
    /* add other fields as you need */
    int ttl;
    struct VarDesc *next;
} *vars;

struct FunctionDesc {
    char var[8];
    int param_num;
    struct FunctionDesc * next;
} *funcs;

void mips32_gen(tac *head, FILE *_fd);
void _mips_iprintf(const char *fmt, ...);
void load_all(struct VarDesc* reg_arr[16],int sp_change);
void save_all(struct VarDesc* reg_arr[16],int sp_change);
void init_varDesc(tac_opd *opd);

#endif // MIPS_H
