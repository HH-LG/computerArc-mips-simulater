#include<stdio.h>
#include<stdint.h>

int main()
{
    uint32_t inst = 0x8000FFFF;
    int16_t imm = 0xFFFF;
    printf("%d\n",inst == (int32_t)(inst));
    return 0;
}