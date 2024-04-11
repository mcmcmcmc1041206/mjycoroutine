#ifndef COCTX_H_
#define COCTX_H

#include <cstddef>

enum
{
    KRBP = 6, //栈底指针
    KRDI = 7, //rdi，第一个参数
    KRSI = 8, //rsi，第二个参数
    KRETAddr = 9, //下一条执行指令的地址
    KRSP = 13 //栈顶指针
};


struct coctx
{
    void* regs[14]; //一个数组，保存了14个寄存器
    size_t stack_size; //协程栈大小
    char* ss_sp; //协程栈指针
};

extern "C" {
extern void coctx_swap(coctx* ,coctx*) asm("coctx_swap");
}

#endif