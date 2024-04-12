#include "coroutine.h"
#include <atomic>
#include <string.h>
#include <iostream>
#include <thread>
#include <assert.h>

static thread_local Coroutine* t_main_coroutine = nullptr;
static thread_local Coroutine* t_cur_coroutine = nullptr;

static std::atomic_int g_cur_coroutine_id {1};
static std::atomic_int g_coroutine_count {0};

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

Coroutine::Coroutine(int size, char* stack_ptr) : m_stack_size(size), m_stack_sp(stack_ptr) {
  assert(stack_ptr);

  if (!t_main_coroutine) {
    t_main_coroutine = new Coroutine();
  }

  m_cor_id = g_cur_coroutine_id++;
  g_coroutine_count++;
  // DebugLog << "coroutine[" << m_cor_id << "] create";
}

Coroutine::Coroutine(int size,std::function<void()> cb)
:m_stack_size(size),m_call_back(cb)
{
    if(!t_main_coroutine)
    {
        t_main_coroutine = new Coroutine();
    }

    m_stack_sp = reinterpret_cast<char*>(malloc(size));
    this->SetCallback(cb);
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
    std::cout<<"create Coroutine success"<<std::endl;
}

Coroutine::~Coroutine()
{
    g_coroutine_count--;
}

void Coroutine::SetCallback(std::function<void()> cb)
{
    m_call_back = cb;
}

void Coroutine::Resume(Coroutine* co)
{
    if(t_cur_coroutine != t_main_coroutine)
    {
        std::cout<<"Errno"<<std::endl;
        return;
    }

    if(t_main_coroutine == nullptr)
    {
        std::cout<<"main coroutine is empty"<<std::endl;
        return;
    }

    if(co == nullptr)
    {
        std::cout<<"co is empty"<<std::endl;
        return;
    }

    t_cur_coroutine = co;
    coctx_swap(&(t_main_coroutine->m_coctx),&(co->m_coctx));
}

void Coroutine::Yield()
{
    if(t_main_coroutine == nullptr)
    {
        std::cout<<"main coroutine is empty"<<std::endl;
        return;
    }

    if(t_cur_coroutine == t_main_coroutine)
    {
        std::cout<<"cur coroutine is main coroutine"<<std::endl;
        return;
    }

    Coroutine* cur_coroutine = t_cur_coroutine;
    t_cur_coroutine = t_main_coroutine;
    coctx_swap(&(cur_coroutine->m_coctx),&(t_main_coroutine->m_coctx));
}

Coroutine* Coroutine::GetCurrentCoroutine()
{
    if(t_cur_coroutine == nullptr)
    {
        t_cur_coroutine = new(Coroutine);
        return t_cur_coroutine;
    }

    return t_cur_coroutine;
}

Coroutine* Coroutine::GetMainCoroutine()
{
    if(t_main_coroutine == nullptr)
    {
        t_main_coroutine = new(Coroutine);
        return t_main_coroutine;
    }

    return t_main_coroutine;
}