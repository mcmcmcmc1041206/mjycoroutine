#include "coroutine.h"
#include <string.h>
#include <thread>

static thread_local Coroutine* t_main_coroutine = nullptr;
static thread_local Coroutine* t_cur_coroutine = nullptr;

//调用协程中的目标函数，结束后返回主协程
void CoFunction(Coroutine* co)
{
    if(co!=nullptr)
    {
        co->m_call_back();
    }

    Coroutine::Yield();
}

Coroutine::Coroutine()
{
    m_cor_id = g_cur_coroutine_id++;
    g_coroutine_count++;
    memset(&m_coctx,0,sizeof(m_coctx));

    t_main_coroutine = this;
    t_cur_coroutine = this;
    //日志打印
}

Coroutine::Coroutine(int size,std::function<void()> cb)
:m_stack_size(size),m_call_back(cb)
{
    if(!t_main_coroutine)
    {
        t_main_coroutine = new Coroutine();
    }

    m_stack_sp = reinterpret_cast<char*>(malloc(size));

    char* top = m_stack_sp+size;
    top = reinterpret_cast<char*>(reinterpret_cast<unsigned long>(top)&-16LL);
    memset(&m_coctx,0,sizeof(m_coctx));
    m_coctx.regs[KRBP] = top;
    m_coctx.regs[KRSP] = top;
    m_coctx.regs[KRETAddr] = reinterpret_cast<char*>(CoFunction);
    m_coctx.regs[KRDI] = reinterpret_cast<char*>(this);

    m_cor_id = g_cur_coroutine_id++;
    g_coroutine_count++;
    //打印日志
}

