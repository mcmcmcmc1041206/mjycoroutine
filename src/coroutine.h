#ifndef COROUTINE_H_
#define COROUTINE_H_
#include <memory>
#include <functional>
#include "coctx.h"

class Coroutine
{
private:
    Coroutine();
public:
    Coroutine(int size,std::function<void()> cb);
    ~Coroutine();
    using ptr = std::shared_ptr<Coroutine>;
    void SetCallback(std::function<void()> cb);
public:
    static void Yield();
    static void Resume(Coroutine* cor);
    static Coroutine* GetCurrentCoroutine();
    static Coroutine* GetMainCoroutine();

private:
    int m_cor_id; //协程Id
    coctx m_coctx; //寄存器组
    int m_stack_size; //在堆区申请的栈空间大小
    char* m_stack_sp;
public:
    std::function<void()> m_call_back;
};

#endif