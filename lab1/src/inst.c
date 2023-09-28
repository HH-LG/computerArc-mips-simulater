#include<stdio.h>
#include"shell.h"

// ----------------------------------R型指令----------------------------------
// 移位操作

// 1.算数左移
void sll(uint8_t rt, uint8_t rd, uint8_t shamt)
{
    if (rd == 0)    // 不能是0寄存器
        return;

    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] << shamt;
}


// 2.算数右移
void srl(uint8_t rt, uint8_t rd, uint8_t shamt)
{    
    if (rd == 0)    // 不能是0寄存器
        return;

    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] >> shamt;
}

// 3.逻辑右移
void sra(uint8_t rt, uint8_t rd, uint8_t shamt)
{
    if (rd == 0)    // 不能是0寄存器
        return;
    NEXT_STATE.REGS[rd] = (CURRENT_STATE.REGS[rt] >> shamt) | (CURRENT_STATE.REGS[rt] << (32 - shamt));
}

// 4.变长左移位
void sllv(uint8_t rs, uint8_t rt, uint8_t rd)
{    
    if (rd == 0)    // 不能是0寄存器
        return;

    uint8_t s = CURRENT_STATE.REGS[rs] & 0b11111;
    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] << s;
}

// 5.变长右移位
void srlv(uint8_t rs, uint8_t rt, uint8_t rd)
{    
    if (rd == 0)    // 不能是0寄存器
        return;

    uint8_t s = CURRENT_STATE.REGS[rs] & 0b11111;
    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] >> s;
}

// 6.变长逻辑右移位
void srav(uint8_t rs, uint8_t rt, uint8_t rd)
{    
    if (rd == 0)    // 不能是0寄存器
        return;

    uint8_t s = CURRENT_STATE.REGS[rs] & 0b11111;
    NEXT_STATE.REGS[rd] = (CURRENT_STATE.REGS[rt] >> s) | (CURRENT_STATE.REGS[rt] << (32-s));
}

// 位运算

// 7.与运算
void and(uint8_t rs, uint8_t rt, uint8_t rd)
{    
    if (rd == 0)    // 不能是0寄存器
        return;

    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] & CURRENT_STATE.REGS[rt];
}

// 8.或运算
void or(uint8_t rs, uint8_t rt, uint8_t rd)
{    
    if (rd == 0)    // 不能是0寄存器
        return;

    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] | CURRENT_STATE.REGS[rt];
}

// 9.异或运算
void xor(uint8_t rs, uint8_t rt, uint8_t rd)
{    
    if (rd == 0)    // 不能是0寄存器
        return;

    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] ^ CURRENT_STATE.REGS[rt];
}

// 10.或非运算
void nor(uint8_t rs, uint8_t rt, uint8_t rd)
{    
    if (rd == 0)    // 不能是0寄存器
        return;

    NEXT_STATE.REGS[rd] = ~ (CURRENT_STATE.REGS[rs] | CURRENT_STATE.REGS[rt]);
}

// 11.小于置位
void slt(uint8_t rs, uint8_t rt, uint8_t rd)
{    
    if (rd == 0)    // 不能是0寄存器
        return;

    NEXT_STATE.REGS[rd] = (int32_t) CURRENT_STATE.REGS[rs] < (int32_t)CURRENT_STATE.REGS[rt] ? 1 : 0;
}

// 12.无符号小于置位
void sltu(uint8_t rs, uint8_t rt, uint8_t rd)
{    
    if (rd == 0)    // 不能是0寄存器
        return;

    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] < CURRENT_STATE.REGS[rt] ? 1 : 0;
}

// 跳转

// 13.跳转寄存器
void jr(uint8_t rs)
{    
    NEXT_STATE.PC = CURRENT_STATE.REGS[rs] - 4;
}

// 14.跳转寄存器并链接
void jalr(uint8_t rs, uint8_t rd)
{    
    uint8_t t_d = rd == 0 ? 31 : rd;
    NEXT_STATE.REGS[t_d] = CURRENT_STATE.PC + 4;
    NEXT_STATE.PC = CURRENT_STATE.REGS[rs];
}

// 运算

// 15.加法
void add(uint8_t rs, uint8_t rt, uint8_t rd)
{
    if (rd == 0)    // 不能是0寄存器
        return;
    
    NEXT_STATE.REGS[rd] = (int32_t) CURRENT_STATE.REGS[rs] + CURRENT_STATE.REGS[rt];
}

// 16.无符号加法
void addu(uint8_t rs, uint8_t rt, uint8_t rd)
{
    if (rd == 0)    // 不能是0寄存器
        return;
    
    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] + CURRENT_STATE.REGS[rt];
}

// 17.减法
void sub(uint8_t rs, uint8_t rt, uint8_t rd)
{
    if (rd == 0)    // 不能是0寄存器
        return;
    
    NEXT_STATE.REGS[rd] = (int32_t)CURRENT_STATE.REGS[rs] - CURRENT_STATE.REGS[rt];
}

// 18.无符号减法
void subu(uint8_t rs, uint8_t rt, uint8_t rd)
{
    if (rd == 0)    // 不能是0寄存器
        return;
    
    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] - CURRENT_STATE.REGS[rt];
}

// 其他算术指令

// 19.乘法
void mult(uint8_t rs, uint8_t rt, uint8_t rd)
{   
    int64_t res = (int64_t)CURRENT_STATE.REGS[rs] * (int64_t)CURRENT_STATE.REGS[rt];
    NEXT_STATE.HI = res >> 32;
    NEXT_STATE.LO = res & 0xFFFFFFFF;
}

// 20.取出hi
void mfhi(uint8_t rd)
{   
    if (rd == 0)    // 不能是0寄存器
        return;

    NEXT_STATE.REGS[rd] = CURRENT_STATE.HI;
}

// 21.取出lo
void mflo(uint8_t rd)
{   
    if (rd == 0)    // 不能是0寄存器
        return;

    NEXT_STATE.REGS[rd] = CURRENT_STATE.LO;
}

// 22.设置hi
void mthi(uint8_t rs)
{   
    NEXT_STATE.HI = CURRENT_STATE.REGS[rs];
}

// 23.设置lo
void mtlo(uint8_t rs)
{   
    NEXT_STATE.LO = CURRENT_STATE.REGS[rs];
}

// 24.乘法无符号
void multu(uint8_t rs, uint8_t rt, uint8_t rd)
{   
    int64_t res = (uint64_t)CURRENT_STATE.REGS[rs] * (uint64_t)CURRENT_STATE.REGS[rt];
    NEXT_STATE.HI = res >> 32;
    NEXT_STATE.LO = res & 0xFFFFFFFF;
}

// 25.除法
void div(uint8_t rs, uint8_t rt, uint8_t rd)
{   
    NEXT_STATE.LO = (int32_t)CURRENT_STATE.REGS[rs] / (int32_t)CURRENT_STATE.REGS[rt];
    NEXT_STATE.HI = (int32_t)CURRENT_STATE.REGS[rs] % (int32_t)CURRENT_STATE.REGS[rt];
}

// 26.无符号除法
void divu(uint8_t rs, uint8_t rt, uint8_t rd)
{   
    NEXT_STATE.LO = (uint32_t)CURRENT_STATE.REGS[rs] / (uint32_t)CURRENT_STATE.REGS[rt];
    NEXT_STATE.HI = (uint32_t)CURRENT_STATE.REGS[rs] % (uint32_t)CURRENT_STATE.REGS[rt];
}

//特殊指令

// 27.系统调用
void syscall()
{
    // printf("here:syscall\n");
    if(CURRENT_STATE.REGS[2] == 0xA)    // v0为10时退出
        RUN_BIT = FALSE;
}
// ----------------------------------I型指令----------------------------------
//跳转指令

// 28.等于跳转
void beq(uint8_t rs, uint8_t rt, int16_t immediate)
{
    printf("%d,%d,%d\n",rs,rt,immediate);
    if(CURRENT_STATE.REGS[rs] == CURRENT_STATE.REGS[rt])
        NEXT_STATE.PC = CURRENT_STATE.PC + (immediate << 2);
    //printf("0x%x",NEXT_STATE.PC);
}

// 29.不等于跳转
void bne(uint8_t rs, uint8_t rt, int16_t immediate)
{
    printf("%d,%d\n",rs,rt);
    if(CURRENT_STATE.REGS[rs] != CURRENT_STATE.REGS[rt])
        NEXT_STATE.PC = CURRENT_STATE.PC + (immediate << 2);
}

// 30.小于等于跳转
void blez(uint8_t rs, int16_t immediate)
{
    if((int32_t)CURRENT_STATE.REGS[rs] <= 0)
        NEXT_STATE.PC = CURRENT_STATE.PC + (immediate << 2);
}

// 31.大于零跳转
void bgtz(uint8_t rs, int16_t immediate)
{
    if((int32_t)CURRENT_STATE.REGS[rs] > 0)
        NEXT_STATE.PC = CURRENT_STATE.PC + (immediate << 2);
}

// 32.小于零跳转
void bltz(uint8_t rs, int16_t immediate)
{
    if ((int32_t)CURRENT_STATE.REGS[rs] < 0)
        NEXT_STATE.PC = CURRENT_STATE.PC + (immediate << 2);
}

// 33.大于等于零跳转
void bgez(uint8_t rs, int16_t immediate)
{
    if ((int32_t)CURRENT_STATE.REGS[rs] >= 0)
        NEXT_STATE.PC = CURRENT_STATE.PC + (immediate << 2);
}

// 34.小于零跳转并链接
void bltzal(uint8_t rs, int16_t immediate)
{
    NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4;
    if ((int32_t)CURRENT_STATE.REGS[rs] < 0)
        NEXT_STATE.PC = CURRENT_STATE.PC + (immediate << 2);
}

// 35.大于等于零跳转并链接
void bgezal(uint8_t rs, int16_t immediate)
{
    NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4;
    if ((int32_t)CURRENT_STATE.REGS[rs] >= 0)
        NEXT_STATE.PC = CURRENT_STATE.PC + (immediate << 2);
}

// 算数指令

// 36.立即数加法
void addi(uint8_t rs, uint8_t rt, int16_t immediate)
{
    if(rt == 0) // 不能是0寄存器
        return;
    
    NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] + (int32_t)immediate;
}

// 37.立即数无符号加法
void addiu(uint8_t rs, uint8_t rt, int16_t immediate)
{
    if(rt == 0) // 不能是0寄存器
        return;
    NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] + (uint32_t)immediate;
    printf("addiu:rs:%d, rt:%d, imm:%d\n", rs, rt, immediate);
}

//位运算

// 38.立即数小于置位
void slti(uint8_t rs, uint8_t rt, int16_t immediate)
{
    if(rt == 0) // 不能是0寄存器
        return;
    
    NEXT_STATE.REGS[rt] = (int32_t)CURRENT_STATE.REGS[rs] < (int32_t)immediate ? 1 : 0;
}

// 39.立即数无符号小于置位
void sltiu(uint8_t rs, uint8_t rt, int16_t immediate)
{
    if(rt == 0) // 不能是0寄存器
        return;
    
    NEXT_STATE.REGS[rt] = (uint32_t)CURRENT_STATE.REGS[rs] < (uint32_t)immediate ? 1 : 0;
}

// 40.立即数与运算
void andi(uint8_t rs, uint8_t rt, int16_t immediate)
{
    if(rt == 0) // 不能是0寄存器
        return;
    
    NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] & immediate & 0x0000FFFF;
}

// 41.立即数或运算
void ori(uint8_t rs, uint8_t rt, int16_t immediate)
{
    if(rt == 0) // 不能是0寄存器
        return;
    
    NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] | (immediate & 0x0000FFFF);
}

// 42.立即数异或运算
void xori(uint8_t rs, uint8_t rt, int16_t immediate)
{
    if(rt == 0) // 不能是0寄存器
        return;
    
    NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] ^ (immediate & 0x0000FFFF);
}

// 43.设置高十六位
void lui(uint8_t rt, int16_t immediate)
{
    if(rt == 0) // 不能是0寄存器
        return;

    NEXT_STATE.REGS[rt] = immediate << 16;
}

// 访存

// 44.加载字节
void lb(uint8_t rs, uint8_t rt, int16_t immediate)
{
    if(rt == 0) // 不能是0寄存器
        return;
    
    uint32_t vaddr = CURRENT_STATE.REGS[rs] + immediate;
    uint32_t val = mem_read_32(vaddr) & 0xFF;
    NEXT_STATE.REGS[rt] = val > 0x80? 0xFFFFFF00 | val : val;
}

// 45.加载半字
void lh(uint8_t rs, uint8_t rt, int16_t immediate)
{
    if(rt == 0) // 不能是0寄存器
        return;
    
    uint32_t vaddr = CURRENT_STATE.REGS[rs] + immediate;
    uint32_t val = mem_read_32(vaddr) & 0xFFFF;
    NEXT_STATE.REGS[rt] = val > 0x8000? 0xFFFF0000 | val : val;
}

// 46.加载字
void lw(uint8_t rs, uint8_t rt, int16_t immediate)
{
    if(rt == 0) // 不能是0寄存器
        return;
    
    uint32_t vaddr = CURRENT_STATE.REGS[rs] + immediate;
    NEXT_STATE.REGS[rt] =  mem_read_32(vaddr);
}

// 47.加载字节无符号
void lbu(uint8_t rs, uint8_t rt, int16_t immediate)
{
    if(rt == 0) // 不能是0寄存器
        return;
    
    uint32_t vaddr = CURRENT_STATE.REGS[rs] + immediate;
    uint32_t val = mem_read_32(vaddr) & 0xFF;
    NEXT_STATE.REGS[rt] = val;
}

// 48.加载半字无符号
void lhu(uint8_t rs, uint8_t rt, int16_t immediate)
{
    if(rt == 0) // 不能是0寄存器
        return;
    
    uint32_t vaddr = CURRENT_STATE.REGS[rs] + immediate;
    uint32_t val = mem_read_32(vaddr) & 0xFFFF;
    NEXT_STATE.REGS[rt] = val;
}

// 49.存储字节
void sb(uint8_t rs, uint8_t rt, int16_t immediate)
{
    uint32_t vaddr = CURRENT_STATE.REGS[rs] + immediate;
    uint32_t val = mem_read_32(vaddr) & 0xFFFFFF00;
    mem_write_32(vaddr,val | (CURRENT_STATE.REGS[rt] & 0xFF));
}

// 50.存储半字
void sh(uint8_t rs, uint8_t rt, int16_t immediate)
{
    uint32_t vaddr = CURRENT_STATE.REGS[rs] + immediate;
    uint32_t val = mem_read_32(vaddr) & 0xFFFF0000;
    mem_write_32(vaddr,val | (CURRENT_STATE.REGS[rt] & 0xFFFF));
}

// 51.存储字
void sw(uint8_t rs, uint8_t rt, int16_t immediate)
{
    uint32_t vaddr = CURRENT_STATE.REGS[rs] + immediate;
    mem_write_32(vaddr, CURRENT_STATE.REGS[rt]);
}

// ----------------------------------J型指令----------------------------------
// 跳转指令

// 52.跳转
void j(uint32_t target)
{
    NEXT_STATE.PC = (((CURRENT_STATE.PC+4) & 0xF0000000) | (target<<2))-4;  //先减去四，以免后面加上四出错
}

// 53.跳转并链接
void jal(uint32_t target)
{
    NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4;
    NEXT_STATE.PC = (((CURRENT_STATE.PC+4) & 0xF0000000) | (target<<2))-4;  //先减去四，以免后面加上四出错
}