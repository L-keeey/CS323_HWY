#include "mips32.h"

/* the output file descriptor, may not be explicitly used */
FILE *fd;

#define _tac_kind(tac) (((tac)->code).kind)
#define _tac_quadruple(tac) (((tac)->code).tac)
#define _reg_name(reg) regs[reg].name

/* 
    there are only four kinds of tac_opd 
    1.OP_LABEL      2.OP_VARIABLE
    3.OP_CONSTANT   4.OP_POINTER 

    1. for J type instructions, OP_LABEL is used
    explicitly no register alloction is need

    2. OP_VARIABLE need set relevent RegDesc->name 
    with its variable name and the register t0-s7
    are the valid registers to allocate

    3. For each TAC, at most two OP_CONSTANT will be
    used continuously, t8, t9 will be used alternately  

    4. since structure and array are not supported
    case kind = OP_POINTER need no consideration

*/

int stack_var_num = 0;
int var_time_counter = 0;

struct VarDesc* find_varDesc(char *var){
    struct VarDesc* curr = vars;
    while (curr->next != NULL){
        curr = curr->next;
        if (strcmp(curr->var, var) == 0)
            return curr;
    }
}

Register find_register(char* var){
    for (int r = t0; r <= s7; r++){
        if (strcmp(regs[r].var, var) == 0)
            return r;
    }
    return zero;
}

Register first_idle_reg(){
    for (int r = t0; r <= s7; r++){
        if (!regs[r].dirty){
            regs[r].dirty = TRUE;
            return r;
        }       
    }
    return zero;
}

Register no_use_reg(){
    for (int r = t0; r <= s7; r++){
        if (!regs[r].ttl){
            return r;
        }
    }
    return zero;
}

Register find_oldest_reg(){
    Register re = t0;
    for (int r = t1; r <= s7; r++){
        if (regs[r].time < regs[re].time)
            re = r;
    }
    return re;
}

void load_reg_var(struct VarDesc* vdx, Register rx){
    vdx->reg = rx;
    strcpy(regs[rx].var, vdx->var);
    regs[rx].time = var_time_counter++;
    regs[rx].ttl = vdx->ttl;
    _mips_iprintf("lw %s, %d($sp)", _reg_name(rx), vdx->offset);
}

void spill_register(Register reg){
    char *var = regs[reg].name;
    struct VarDesc* vd = find_varDesc(var);
    vd->reg = zero;
    // TODO: discuss how yo maintain the stack
    int offset = 4 * stack_var_num++; 
    vd->offset = offset;
    _mips_iprintf("sw %s, %d($sp)", _reg_name(reg), offset);
}

Register get_register(tac_opd *opd){
    assert(opd->kind == OP_VARIABLE);
    char *var = opd->char_val;
    /* COMPLETE the register allocation */
    // find the register that contains the opd
    struct VarDesc* vdx = find_varDesc(var);
    Register reg = NUM_REGS;
    vdx->ttl--;

    // some register contains the ropd
    if (vdx->reg != zero){
        reg = vdx->reg;
        regs[reg].ttl--;
    }
    // no register contains the ropd
    else{
        // some idle register can be used
        Register idle = first_idle_reg();
        if (idle != zero)
            reg = idle;

        // all the registers are ocupied
        else {
            // use the reg that is no use later
            Register nu = no_use_reg();
            if (nu != zero)
                reg = nu;

            // spill the oldest var and reuse the register
            else{
                reg = find_oldest_reg();
                spill_register(reg);
            }
        }
        load_reg_var(vdx, idle);
    }

    return reg;
}

Register get_register_o(tac_opd *lopd, tac_opd *ropd){
    assert(ropd->kind == OP_VARIABLE);
    char *var = ropd->char_val;
    /* COMPLETE the register allocation */
    // find the register that contains the ropd

    // some register contains the ropd
    struct VarDesc* vdx = find_varDesc(var);
    Register reg = NUM_REGS;
    vdx->ttl--;

    // some register contains the ropd
    if (vdx->reg != zero){
        reg = vdx->reg;
        regs[reg].ttl--;
    }
    // no register contains the ropd
    else{
        // some idle register can be used
        Register idle = first_idle_reg();
        if (idle != zero)
            reg = idle;

        // all the registers are ocupied
        else {
            //load without spill
            // case 1: use the reg containig lopt 
            assert(lopd->kind == OP_VARIABLE);
            Register lr = find_register(lopd->char_val);
            if (lr != zero)
                reg = lr;

            // case 2: use the reg that is no use later
            else{
                Register nu = no_use_reg();
                if (nu != zero)
                    reg = nu;

                // spill the oldest var and reuse the register
                else{
                    reg = find_oldest_reg();
                    spill_register(reg);
                }
            }
        }
        load_reg_var(vdx, idle);
    }

    return reg;
}

Register get_register_w(tac_opd *opd){
    // find an idle register to hold the opd

    /* COMPLETE the register allocation (for write) */
    if (opd->kind == OP_CONSTANT){
        if (!regs[t8].dirty && !regs[t9].dirty){
            regs[t8].dirty = TRUE;
            return t8;
        }else if (regs[t8].dirty && !regs[t9].dirty){
            regs[t9].dirty = TRUE;
            return t9;
        }else if (regs[t8].dirty && regs[t9].dirty){
            regs[t8].dirty = FALSE;
            return t8;
        }else if (!regs[t8].dirty && regs[t9].dirty){
            regs[t9].dirty = FALSE;
            return t9;
        }
    } else if (opd->kind == OP_VARIABLE){
        return get_register(opd);
    }
}

void _mips_printf(const char *fmt, ...){
    va_list args;
    va_start(args, fmt);
    vfprintf(fd, fmt, args);
    va_end(args);
    fputs("\n", fd);
}

void _mips_iprintf(const char *fmt, ...){
    va_list args;
    fputs("  ", fd); // `iprintf` stands for indented printf
    va_start(args, fmt);
    vfprintf(fd, fmt, args);
    va_end(args);
    fputs("\n", fd);
}


/* PARAM: a pointer to `struct tac_node` instance
   RETURN: the next instruction to be translated */
tac *emit_label(tac *label){
    assert(_tac_kind(label) == LABEL);
    _mips_printf("label%d:", _tac_quadruple(label).labelno->int_val);
    return label->next;
}

tac *emit_function(tac *function){
    _mips_printf("%s:", _tac_quadruple(function).funcname);
    return function->next;
}

tac *emit_assign(tac *assign){
    Register x, y;

    x = get_register_w(_tac_quadruple(assign).left);
    if(_tac_quadruple(assign).right->kind == OP_CONSTANT){
        _mips_iprintf("li %s, %d", _reg_name(x),
                                   _tac_quadruple(assign).right->int_val);
    }
    else{
        y = get_register_o(_tac_quadruple(assign).left, _tac_quadruple(assign).right);
        _mips_iprintf("move %s, %s", _reg_name(x), _reg_name(y));
    }
    return assign->next;
}

tac *emit_add(tac *add){
    Register x, y, z;

    x = get_register_w(_tac_quadruple(add).left);
    if(_tac_quadruple(add).r1->kind == OP_CONSTANT){
        y = get_register_o(_tac_quadruple(add).left, _tac_quadruple(add).r2);
        _mips_iprintf("addi %s, %s, %d", _reg_name(x),
                                         _reg_name(y),
                                         _tac_quadruple(add).r1->int_val);
    }
    else if(_tac_quadruple(add).r2->kind == OP_CONSTANT){
        y = get_register_o(_tac_quadruple(add).left, _tac_quadruple(add).r1);
        _mips_iprintf("addi %s, %s, %d", _reg_name(x),
                                         _reg_name(y),
                                         _tac_quadruple(add).r2->int_val);
    }
    else{
        y = get_register_o(_tac_quadruple(add).left, _tac_quadruple(add).r1);
        z = get_register_o(_tac_quadruple(add).left, _tac_quadruple(add).r2);
        _mips_iprintf("add %s, %s, %s", _reg_name(x),
                                        _reg_name(y),
                                        _reg_name(z));
    }
    return add->next;
}

tac *emit_sub(tac *sub){
    Register x, y, z;

    x = get_register_w(_tac_quadruple(sub).left);
    if(_tac_quadruple(sub).r1->kind == OP_CONSTANT){
        y = get_register_o(_tac_quadruple(sub).left, _tac_quadruple(sub).r2);
        _mips_iprintf("neg %s, %s", _reg_name(y), _reg_name(y));
        _mips_iprintf("addi %s, %s, %d", _reg_name(x),
                                         _reg_name(y),
                                         _tac_quadruple(sub).r1->int_val);
    }
    else if(_tac_quadruple(sub).r2->kind == OP_CONSTANT){
        y = get_register_o(_tac_quadruple(sub).left, _tac_quadruple(sub).r1);
        _mips_iprintf("addi %s, %s, -%d", _reg_name(x),
                                          _reg_name(y),
                                          _tac_quadruple(sub).r2->int_val);
    }
    else{
        y = get_register_o(_tac_quadruple(sub).left, _tac_quadruple(sub).r1);
        z = get_register_o(_tac_quadruple(sub).left, _tac_quadruple(sub).r2);
        _mips_iprintf("sub %s, %s, %s", _reg_name(x),
                                        _reg_name(y),
                                        _reg_name(z));
    }
    return sub->next;
}

tac *emit_mul(tac *mul){
    Register x, y, z;

    x = get_register_w(_tac_quadruple(mul).left);
    if(_tac_quadruple(mul).r1->kind == OP_CONSTANT){
        y = get_register_w(_tac_quadruple(mul).r1);
        z = get_register_o(_tac_quadruple(mul).left, _tac_quadruple(mul).r2);
        _mips_iprintf("lw %s, %d", _reg_name(y),
                                   _tac_quadruple(mul).r1->int_val);
    }
    else if(_tac_quadruple(mul).r2->kind == OP_CONSTANT){
        y = get_register_o(_tac_quadruple(mul).left, _tac_quadruple(mul).r1);
        z = get_register_w(_tac_quadruple(mul).r2);
        _mips_iprintf("lw %s, %d", _reg_name(z),
                                   _tac_quadruple(mul).r2->int_val);
    }
    else{
        y = get_register_o(_tac_quadruple(mul).left, _tac_quadruple(mul).r1);
        z = get_register_o(_tac_quadruple(mul).left, _tac_quadruple(mul).r2);
    }
    _mips_iprintf("mul %s, %s, %s", _reg_name(x),
                                    _reg_name(y),
                                    _reg_name(z));
    return mul->next;
}

tac *emit_div(tac *div){
    Register x, y, z;

    x = get_register_w(_tac_quadruple(div).left);
    if(_tac_quadruple(div).r1->kind == OP_CONSTANT){
        y = get_register_w(_tac_quadruple(div).r1);
        z = get_register_o(_tac_quadruple(div).left, _tac_quadruple(div).r2);
        _mips_iprintf("lw %s, %d", _reg_name(y),
                                   _tac_quadruple(div).r1->int_val);
    }
    else if(_tac_quadruple(div).r2->kind == OP_CONSTANT){
        y = get_register_o(_tac_quadruple(div).left, _tac_quadruple(div).r1);
        z = get_register_w(_tac_quadruple(div).r2);
        _mips_iprintf("lw %s, %d", _reg_name(z),
                                   _tac_quadruple(div).r2->int_val);
    }
    else{
        y = get_register_o(_tac_quadruple(div).left, _tac_quadruple(div).r1);
        z = get_register_o(_tac_quadruple(div).left, _tac_quadruple(div).r2);
    }
    _mips_iprintf("div %s, %s", _reg_name(y), _reg_name(z));
    _mips_iprintf("mflo %s", _reg_name(x));
    return div->next;
}

tac *emit_addr(tac *addr){
    Register x, y;

    x = get_register_w(_tac_quadruple(addr).left);
    y = get_register(_tac_quadruple(addr).right);
    _mips_iprintf("move %s, %s", _reg_name(x), _reg_name(y));
    return addr->next;
}

tac *emit_fetch(tac *fetch){
    Register x, y;

    x = get_register_w(_tac_quadruple(fetch).left);
    y = get_register(_tac_quadruple(fetch).raddr);
    _mips_iprintf("lw %s, 0(%s)", _reg_name(x), _reg_name(y));
    return fetch->next;
}

tac *emit_deref(tac *deref){
    Register x, y;

    x = get_register(_tac_quadruple(deref).laddr);
    y = get_register(_tac_quadruple(deref).right);
    _mips_iprintf("sw %s, 0(%s)", _reg_name(y), _reg_name(x));
    return deref->next;
}

tac *emit_goto(tac *goto_){
    _mips_iprintf("j label%d", _tac_quadruple(goto_).labelno->int_val);
    return goto_->next;
}

tac *emit_iflt(tac *iflt){
    /* COMPLETE emit function */
    return iflt->next;
}

tac *emit_ifle(tac *ifle){
    /* COMPLETE emit function */
    return ifle->next;
}

tac *emit_ifgt(tac *ifgt){
    /* COMPLETE emit function */
    return ifgt->next;
}

tac *emit_ifge(tac *ifge){
    /* COMPLETE emit function */
    return ifge->next;
}

tac *emit_ifne(tac *ifne){
    /* COMPLETE emit function */
    return ifne->next;
}

tac *emit_ifeq(tac *ifeq){
    /* COMPLETE emit function */
    return ifeq->next;
}

tac *emit_return(tac *return_){
    /* COMPLETE emit function */
    return return_->next;
}

tac *emit_dec(tac *dec){
    /* NO NEED TO IMPLEMENT */
    return dec->next;
}

tac *emit_arg(tac *arg){
    /* COMPLETE emit function */
    return arg->next;
}

tac *emit_call(tac *call){
    /* COMPLETE emit function */
    return call->next;
}

tac *emit_param(tac *param){
    /* COMPLETE emit function */
    return param->next;
}

tac *emit_read(tac *read){
    Register x = get_register_w(_tac_quadruple(read).p);

    _mips_iprintf("addi $sp, $sp, -4");
    _mips_iprintf("sw $ra, 0($sp)");
    _mips_iprintf("jal read");
    _mips_iprintf("lw $ra, 0($sp)");
    _mips_iprintf("addi $sp, $sp, 4");
    _mips_iprintf("move %s, $v0", _reg_name(x));
    return read->next;
}

tac *emit_write(tac *write){
    Register x = get_register(_tac_quadruple(write).p);

    _mips_iprintf("move $a0, %s", _reg_name(x));
    _mips_iprintf("addi $sp, $sp, -4");
    _mips_iprintf("sw $ra, 0($sp)");
    _mips_iprintf("jal write");
    _mips_iprintf("lw $ra, 0($sp)");
    _mips_iprintf("addi $sp, $sp, 4");
    return write->next;
}

void emit_preamble(){
    _mips_printf("# SPL compiler generated assembly");
    _mips_printf(".data");
    _mips_printf("_prmpt: .asciiz \"Enter an integer: \"");
    _mips_printf("_eol: .asciiz \"\\n\"");
    _mips_printf(".globl main");
    _mips_printf(".text");
}

void emit_read_function(){
    _mips_printf("read:");
    _mips_iprintf("li $v0, 4");
    _mips_iprintf("la $a0, _prmpt");
    _mips_iprintf("syscall");
    _mips_iprintf("li $v0, 5");
    _mips_iprintf("syscall");
    _mips_iprintf("jr $ra");
}

void emit_write_function(){
    _mips_printf("write:");
    _mips_iprintf("li $v0, 1");
    _mips_iprintf("syscall");
    _mips_iprintf("li $v0, 4");
    _mips_iprintf("la $a0, _eol");
    _mips_iprintf("syscall");
    _mips_iprintf("move $v0, $0");
    _mips_iprintf("jr $ra");
}

static tac* (*emitter[])(tac*) = {
    emit_label, emit_function, emit_assign,
    emit_add, emit_sub, emit_mul, emit_div,
    emit_addr, emit_fetch, emit_deref, emit_goto,
    emit_iflt, emit_ifle, emit_ifgt, emit_ifge, emit_ifne, emit_ifeq,
    emit_return, emit_dec, emit_arg, emit_call, emit_param,
    emit_read, emit_write
};

tac *emit_code(tac *head){
    tac *(*tac_emitter)(tac*);
    tac *tac_code = head;
    emit_preamble();
    emit_read_function();
    emit_write_function();
    while(tac_code != NULL){
        if(_tac_kind(tac_code) != NONE){
            tac_emitter = emitter[_tac_kind(tac_code)];
            tac_code = tac_emitter(tac_code);
        }
        else{
            tac_code = tac_code->next;
        }
    }
}

void init_varDesc(tac_opd *opd){
    if (opd->kind != OP_VARIABLE)
        return;
    
    char *var = opd->char_val;
    struct VarDesc* curr = vars;
    while (curr->next != NULL){
        curr = curr->next;
        if (strcmp(curr->var, var) == 0)
            curr->ttl++;
    }

    struct VarDesc* new_var = (struct VarDesc*)malloc(sizeof(struct VarDesc));
    strcpy(new_var->var, var);
    new_var->next = NULL;
    new_var->ttl = 1;
    curr->next = new_var;

}

void init_varDescs(tac *head){
    tac *tac_code = head;
     while(tac_code != NULL){
        switch (_tac_kind(tac_code))
        {
        case ASSIGN:
            init_varDesc(tac_code->code.assign.left);
            init_varDesc(tac_code->code.assign.right);
            break;
        case ADD:
            init_varDesc(tac_code->code.add.left);
            init_varDesc(tac_code->code.add.r1);
            init_varDesc(tac_code->code.add.r2);
            break;
        case SUB:
            init_varDesc(tac_code->code.sub.left);
            init_varDesc(tac_code->code.sub.r1);
            init_varDesc(tac_code->code.sub.r2);
            break;
        case MUL:
            init_varDesc(tac_code->code.mul.left);
            init_varDesc(tac_code->code.mul.r1);
            init_varDesc(tac_code->code.mul.r2);
            break;
        case DIV:
            init_varDesc(tac_code->code.div.left);
            init_varDesc(tac_code->code.div.r1);
            init_varDesc(tac_code->code.div.r2);
            break;
        case ADDR:
            init_varDesc(tac_code->code.addr.left);
            init_varDesc(tac_code->code.addr.right);
            break;
        case FETCH:
            init_varDesc(tac_code->code.fetch.left);
            init_varDesc(tac_code->code.fetch.raddr);
            break;
        case DEREF:
            init_varDesc(tac_code->code.deref.laddr);
            init_varDesc(tac_code->code.deref.right);
            break;
        case IFLT:
            init_varDesc(tac_code->code.iflt.c1);
            init_varDesc(tac_code->code.iflt.c2);
            break;
        case IFLE:
            init_varDesc(tac_code->code.ifle.c1);
            init_varDesc(tac_code->code.ifle.c2);
            break;
        case IFGT:
            init_varDesc(tac_code->code.ifgt.c1);
            init_varDesc(tac_code->code.ifgt.c2);
            break;
        case IFGE:
            init_varDesc(tac_code->code.ifge.c1);
            init_varDesc(tac_code->code.ifge.c2);
            break;
        case IFEQ:
            init_varDesc(tac_code->code.ifeq.c1);
            init_varDesc(tac_code->code.ifeq.c2);
            break;
        case IFNE:
            init_varDesc(tac_code->code.ifne.c1);
            init_varDesc(tac_code->code.ifne.c2);
            break;    
        case RETURN:
            init_varDesc(tac_code->code.return_.var);
            break;
        case CALL:
            init_varDesc(tac_code->code.call.ret);
            break;
        case READ:
            init_varDesc(tac_code->code.read.p);
            break;
        case WRITE:
            init_varDesc(tac_code->code.write.p);
            break;                                                    
        default:
            break;
        }

        tac_code = tac_code->next;        
    }
}

/* translate a TAC list into mips32 assembly
   output the textual assembly code to _fd */
void mips32_gen(tac *head, FILE *_fd){
    regs[zero].name = "$zero";
    regs[at].name = "$at";
    regs[v0].name = "$v0"; regs[v1].name = "$v1";
    regs[a0].name = "$a0"; regs[a1].name = "$a1";
    regs[a2].name = "$a2"; regs[a3].name = "$a3";
    regs[t0].name = "$t0"; regs[t1].name = "$t1";
    regs[t2].name = "$t2"; regs[t3].name = "$t3";
    regs[t4].name = "$t4"; regs[t5].name = "$t5";
    regs[t6].name = "$t6"; regs[t7].name = "$t7";
    regs[s0].name = "$s0"; regs[s1].name = "$s1";
    regs[s2].name = "$s2"; regs[s3].name = "$s3";
    regs[s4].name = "$s4"; regs[s5].name = "$s5";
    regs[s6].name = "$s6"; regs[s7].name = "$s7";
    regs[t8].name = "$t8"; regs[t9].name = "$t9";
    regs[k0].name = "$k0"; regs[k1].name = "$k1";
    regs[gp].name = "$gp";
    regs[sp].name = "$sp"; regs[fp].name = "$fp";
    regs[ra].name = "$ra";
    vars = (struct VarDesc*)malloc(sizeof(struct VarDesc));
    vars->next = NULL;
    fd = _fd;
    emit_code(head);
}
