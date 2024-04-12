#include "coroutine_pool.h"
#include <iostream>
#include <memory>
#include <pthread.h>
#include <unistd.h>
#include "MutexLock.h"

Coroutine::ptr cor;
Coroutine::ptr cor2;

Mutex m_mutex;

void fun1()
{
    std::cout << "cor1 ---- now fitst resume fun1 coroutine by thread 1" << std::endl;
    std::cout << "cor1 ---- now begin to yield fun1 coroutine" << std::endl;
    {
        MutexLockGuard mutexguard(m_mutex);

        std::cout << "cor1 ---- coroutine lock on test_, sleep 5s begin" << std::endl;

        sleep(5);
        std::cout << "cor1 ---- sleep 5s end, now back coroutine lock" << std::endl;
    }
    
    Coroutine::Yield();
    std::cout << "cor1 ---- fun1 coroutine back, now end" << std::endl;
}

void fun2()
{
    std::cout << "cor222 ---- now fitst resume fun1 coroutine by thread 1" << std::endl;
    std::cout << "cor222 ---- now begin to yield fun1 coroutine" << std::endl;

    sleep(2);
    std::cout << "cor222 ---- coroutine2 want to get coroutine lock of test_" << std::endl;
    MutexLockGuard mutexguard(m_mutex);

    std::cout << "cor222 ---- coroutine2 get coroutine lock of test_ succ" << std::endl;
}

void* thread1_func(void*) {
  std::cout << "thread 1 begin" << std::endl;
  std::cout << "now begin to resume fun1 coroutine in thread 1" << std::endl;
  Coroutine::Resume(cor.get());
  std::cout << "now fun1 coroutine back in thread 1"<< std::endl;
  std::cout << "thread 1 end" << std::endl;
  return NULL;
}

void* thread2_func(void*)
{
    Coroutine::GetCurrentCoroutine();
    std::cout << "thread 2 begin" << std::endl;
    std::cout << "now begin to resume fun1 coroutine in thread 2" << std::endl;
    Coroutine::Resume(cor2.get());
    std::cout << "now fun1 coroutine back in thread 2" << std::endl;
    std::cout << "thread 2 end" << std::endl;
    return NULL;
}

int main(int argc,char* argv[])
{
    std::cout<<"main begin"<<std::endl;
    int stack_size = 128* 1024;
    int pool_size = 5;
    //初始化协程池
    CoroutinePool coroutinepool(pool_size,stack_size);
    std::cout<<"succeessful create coroutinepool"<<std::endl;
    cor = coroutinepool.getCoroutineInstance();
    cor2 = coroutinepool.getCoroutineInstance();
    cor->SetCallback(fun1);
    cor2->SetCallback(fun2);

    pthread_t thread2;
    pthread_create(&thread2,NULL,&thread2_func,NULL);

    thread1_func(NULL);
    pthread_join(thread2,NULL);

    coroutinepool.returnCoroutine(cor);
    coroutinepool.returnCoroutine(cor2);
    std::cout<<"main end"<<std::endl;
}