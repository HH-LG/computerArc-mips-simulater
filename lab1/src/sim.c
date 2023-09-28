#include <stdio.h>
#include "shell.h"
#include "inst.h"
#include "sim.h"

// 读取指定位数的比特位
int32_t read_bits(uint32_t instruction, int start, int length)
{
    return (instruction>>start) & ((1<<length)-1);
}

// r型指令
void rtype(uint8_t rs, uint8_t rt, uint8_t rd, uint8_t shamt, uint8_t funct) {
	// printf("in rtype:rs:%d, rt:%d, rd:%d, shamt:%d, funct:%d\n",rs,rt,rd,shamt,funct);
	if(funct <= 7)	//根据funct判断是哪种移位指令
		r_shift(rs, rt, rd, shamt, funct);
	else if( (funct >= 24 && funct <= 27) || (funct >= 32 && funct <= 35) )
		r_arithmetic(rs, rt, rd, funct);
	else if(funct >= 36 && funct <= 39)
		r_logical(rs, rt, rd, funct);
	else if(funct == 42 || funct == 43)
		r_conditional(rs, rt, rd, funct);
	else if(funct == 8 || funct == 9)
		r_jump(rs, rd, funct);
	else if(funct >= 16  && funct <= 19)
		r_lo_hi(rs, rd, funct);
	else if(funct == 12)
	{
		syscall();
	}
    else
        ;
}

// i型指令
void itype(uint8_t op, uint8_t rs, uint8_t rt, int16_t imm) {
	if(op >= 4 && op <= 7)	//根据op判断是哪种分支指令
		i_branches(op, rs, rt, imm);
	else if(op >= 8 && op <= 15)
		i_arithmetic(op, rs, rt, imm);
	else if(op >= 32 && op <= 43) 
		i_mem(op, rs, rt, imm);
    else
        ;
}

// j型指令
void jtype(uint8_t op, uint32_t target) {
	switch(op) {	//根据op判断是哪种跳转指令
		case J:
			j(target);
			return;
		case JAL:
			jal(target);
			return;
	}
}

// 移位指令
void r_shift(uint8_t rs, uint8_t rt, uint8_t rd, uint8_t shamt, uint8_t funct) {
	switch(funct) {	//根据funct判断是哪种移位指令
		case SLL:
			sll(rt, rd, shamt);
			return;
		case SRL:
			srl(rt, rd, shamt);
			return;
		case SRA:
			sra(rt, rd, shamt);
			return;
		case SLLV:
			sllv(rs, rt, rd);
			return;
		case SRLV:
			srlv(rs, rt, rd);
			return;
		case SRAV:
			srav(rs, rt, rd);
			return;
	}
}

// 算数指令
void r_arithmetic(uint8_t rs, uint8_t rt, uint8_t rd, uint8_t funct) {
	switch(funct) {	//根据funct判断是哪种算数指令
		case ADD:
			add(rs, rt, rd);
			return;
		case ADDU:
			addu(rs, rt, rd);
			return;
		case SUB:
			sub(rs, rt, rd);
			return;
		case SUBU:
			subu(rs, rt, rd);
			return;
		case MULT:
			mult(rs, rt);
			return;
		case MULTU:
			multu(rs, rt);
			return;
		case DIV:
			div(rs, rt);
			return;
		case DIVU:
			divu(rs, rt);
			return;
	}
}

// 逻辑指令
void r_logical(uint8_t rs, uint8_t rt, uint8_t rd, uint8_t funct) {
	switch(funct) {	//根据funct判断是哪种逻辑指令
		case AND:
			and(rs, rt, rd);
			return;
		case OR:
			or(rs, rt, rd);
			return;
		case XOR:
			xor(rs, rt, rd);
			return;
		case NOR:
			nor(rs, rt, rd);
			return;
	}
}

// 条件指令
void r_conditional(uint8_t rs, uint8_t rt, uint8_t rd, uint8_t funct) {
	switch(funct) {	//根据funct判断是哪种条件指令
		case SLT:
			slt(rs, rt, rd);
			return;
		case SLTU:
			sltu(rs, rt, rd);
			return;
	}
}

// r型跳转指令
void r_jump(uint8_t rs, uint8_t rd, uint8_t funct) {
	switch(funct) {	//根据funct判断是哪种r型跳转指令
		case JR:
			jr(rs);
			return;
		case JALR:
			jalr(rs, rd);
			return;
	}
}

// r型与hi，lo相关指令
void r_lo_hi(uint8_t rs, uint8_t rd, uint8_t funct) {
	switch(funct) {	//根据funct判断是哪种指令
		case MFHI:
			mfhi(rd);
			return;
		case MFLO:
			mflo(rd);
			return;
		case MTHI:
			mthi(rs);
			return;
		case MTLO:
			mtlo(rs);
			return;
	}
}

// i型分支指令，比较特殊，op是1
void itype_jump(uint8_t rs, uint8_t rt, int16_t imm) {
	switch(rt) {	//根据rt判断是哪种分支指令
		case BLTZ:
			bltz(rs, imm);
			return;
		case BGEZ:
			bgez(rs, imm);
			return;
		case BLTZAL:
			bltzal(rs, imm);
			return;
		case BGEZAL:
			bgezal(rs, imm);
			return;
	}
}

// i型算数指令
void i_arithmetic(uint8_t op, uint8_t rs, uint8_t rt, int16_t imm) {
	switch(op) {	//根据op判断是哪种算数指令
		case ADDI:
			addi(rs, rt, imm);
			return;
		case ADDIU:
			addiu(rs, rt, imm);
			return;
		case SLTI:
			slti(rs, rt, imm);
			return;
		case SLTIU:
			sltiu(rs, rt, imm);
			return;
		case ANDI:
			andi(rs, rt, imm);
			return;
		case ORI:
			ori(rs, rt, imm);
			return;
		case XORI:
			xori(rs, rt, imm);
			return;
		case LUI:
			lui(rt, imm);
			return;
	}
}

// i型分支指令
void i_branches(uint8_t op, uint8_t rs, uint8_t rt, int16_t imm) {
	switch(op) { //根据op判断是哪种分支指令
		case BEQ:
			beq(rs, rt, imm);
			return;
		case BNE:
			bne(rs, rt, imm);
			return;
		case BLEZ:
			blez(rs, imm);
			return;
		case BGTZ:
			bgtz(rs, imm);
			return;
	}
}

// i型访存指令
void i_mem(uint8_t op, uint8_t base, uint8_t rt, int16_t imm) {
	switch(op) { //根据op判断是哪种访存指令
		case LB:
			lb(base, rt, imm);
			return;
		case LH:
			lh(base, rt, imm);
			return;
		case LW:
			lw(base, rt, imm);
			return;
		case LBU:
			lbu(base, rt, imm);
			return;
		case LHU:
			lhu(base, rt, imm);
			return;
		case SB:
			sb(base, rt, imm);
			return;
		case SH:
			sh(base, rt, imm);
			return;
		case SW:
			sw(base, rt, imm);
			return;
	}
}

void process_instruction()
{
    /* execute one instruction here. You should use CURRENT_STATE and modify
     * values in NEXT_STATE. You can call mem_read_32() and mem_write_32() to
     * access memory. */

    uint32_t instruction;                 //当前指令
    uint8_t op, rs, rt, rd, shamt, funct; // R型
    int16_t immediate;                    // I型
    uint32_t target;                      // J型

    // 读指令
    instruction = mem_read_32(CURRENT_STATE.PC);
	printf("pc:0x%x, instruction: 0x%x\n", CURRENT_STATE.PC, instruction);

    // 初始化R型
    op = read_bits(instruction, 26, 6);
    rs = read_bits(instruction, 21, 5);
    rt = read_bits(instruction, 16, 5);
    rd = read_bits(instruction, 11, 5);
    shamt = read_bits(instruction, 6, 5);
    funct = read_bits(instruction, 0, 6);
	// printf("op:%d, rs:%d, rt:%d, rd:%d, shamt:%d, funct:%d\n",op,rs,rt,rd,shamt,funct);
    // 初始化I型
    immediate = read_bits(instruction, 0, 16);

    // 初始化J型
    target = read_bits(instruction, 0, 26);

	// 执行指令
	switch (op)
	{
	case 0:		// R型
		rtype(rs, rt, rd, shamt, funct);
		break;
	case 1:		// I型特殊跳转指令
		itype_jump(rs, rt, immediate);
		break;
	case 2:		// J型
	case 3:
		jtype(op, target);
		break;
	default:	// I型
		itype(op, rs, rt, immediate);
		break;
	}

	// 更新PC
    NEXT_STATE.PC += 4;	
}
