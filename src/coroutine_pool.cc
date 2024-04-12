#include "coroutine_pool.h"

static CoroutinePool* t_coroutine_container_ptr = nullptr;
int pool_size = 500;
CoroutinePool* CoroutinePool::GetCoroutinePool()
{
    if(!t_coroutine_container_ptr)
    {
        t_coroutine_container_ptr = new CoroutinePool(pool_size);
        return t_coroutine_container_ptr;
    }
    return t_coroutine_container_ptr;
}

CoroutinePool::CoroutinePool(int pool_size, int stack_size /*= 1024 * 128 B*/) : m_pool_size(pool_size), m_stack_size(stack_size)
{
    Coroutine::GetCurrentCoroutine();

    m_memory_pool.push_back(std::make_shared<Memory>(stack_size,pool_size));

    Memory::ptr tmp = m_memory_pool[0];

    for(int i = 0;i<pool_size;i++)
    {
        Coroutine::ptr cor = std::make_shared<Coroutine>(stack_size,tmp->getBlock());
        cor->SetIndex(i);
        m_free_cors.push_back(std::make_pair(cor, false));
    }
}

CoroutinePool::~CoroutinePool(){}

Coroutine::ptr CoroutinePool::getCoroutineInstance()
{
    {
        MutexLockGuard mutexlockguard(m_mutex);
        for(int i = 0;i<m_pool_size;i++)
        {
            if(!m_free_cors[i].second)
            {
                m_free_cors[i].second = true;
                Coroutine::ptr cor = m_free_cors[i].first;
                return cor;
            }
        }
    }
    
    for(size_t i = 0;i<m_memory_pool.size();i++)
    {
        char* tmp = m_memory_pool[i]->getBlock();
        if(tmp)
        {
            Coroutine::ptr cor = std::make_shared<Coroutine>(m_stack_size,tmp);
            return cor;
        }
    }
    m_memory_pool.push_back(std::make_shared<Memory>(m_stack_size,m_pool_size));
    return std::make_shared<Coroutine>(m_stack_size,m_memory_pool[m_memory_pool.size()-1]->getBlock());
}

void CoroutinePool::returnCoroutine(Coroutine::ptr cor)
{
    int i = cor->getIndex();
    if(i>=0 && i<m_pool_size)
    {
        m_free_cors[i].second = false;
    }else
    {
        for(size_t i =1;i<m_memory_pool.size();i++)
        {
            if(m_memory_pool[i]->hasBlock(cor->getStackPtr()))
            {
                m_memory_pool[i]->backBlock(cor->getStackPtr());
            }
        }
    }
    std::cout<<cor->getcorid()<<"has been return"<<std::endl;
}