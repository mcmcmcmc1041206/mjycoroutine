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
    Coroutine(int size,char* stack_ptr);
    Coroutine(int size,std::function<void()> cb);
    ~Coroutine();
    using ptr = std::shared_ptr<Coroutine>;
    void SetCallback(std::function<void()> cb);

    int getIndex(){return m_index;}
    void SetIndex(int i){m_index = i;}

    char* getStackPtr(){return m_stack_sp;}
    int getStackSize(){return m_stack_size;}
    int getcorid(){return m_cor_id;}

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
    int m_index {-1};
    bool m_can_resume {true};
public:
    std::function<void()> m_call_back;
};

#endif