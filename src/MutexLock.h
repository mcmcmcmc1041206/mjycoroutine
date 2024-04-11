#ifndef MUTEX_H_
#define MUTEX_H_

#include <mutex>

class Mutex
{
public:
    Mutex()
    {
        pthread_mutex_init(&mutex_,NULL);
    }

    void lock()
    {
        pthread_mutex_lock(&mutex_);
    }

    void unlock()
    {
        pthread_mutex_unlock(&mutex_);
    }

    ~Mutex()
    {
        pthread_mutex_destroy(&mutex_);
    }

    pthread_mutex_t* Get()
    {
        return &mutex_;
    }
private:
    pthread_mutex_t mutex_;
};

class MutexLockGuard
{
public:
    MutexLockGuard(Mutex& mutex):mutex_(mutex)
    {
        mutex_.lock();
    }

    ~MutexLockGuard()
    {
        mutex_.unlock();
    }

private:
    Mutex mutex_;
};

#endif