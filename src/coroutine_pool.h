#ifndef COROUTINE_POOL_H_
#define COROUTINE_POOL_H_

#include "coroutine.h"
#include "Memory.h"
#include <vector>
#include <iostream>


class CoroutinePool
{
public:
    CoroutinePool(int pool_size,int stack_size = 1024*128);
    ~CoroutinePool();

    Coroutine::ptr getCoroutineInstance();

    void returnCoroutine(Coroutine::ptr cor);

private:
    static CoroutinePool* GetCoroutinePool();
    int m_pool_size {0};
    int m_stack_size {0};

    std::vector<std::pair<Coroutine::ptr,bool>> m_free_cors;

    Mutex m_mutex;

    std::vector<Memory::ptr> m_memory_pool;
};

#endif