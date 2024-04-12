#include "Memory.h"
#include <iostream>

Memory::Memory(int block_size, int block_count):m_block_size(block_size),m_block_count(block_count)
{
    int size = block_size*block_count;
    m_start = (char*)malloc(size);
    m_end = m_start + size - 1;
    std::cout<<"successful mmap"<<std::endl;
    m_blocks.resize(block_size);
    for(int i = 0;i<m_block_size;i++)
    {
        m_blocks[i] = false;
    }
    m_ref_counts = 0;
}

Memory::~Memory() {
  if (!m_start || m_start == (void*)-1) {
    return;
  }
  free(m_start);
  m_start = NULL;
  m_ref_counts = 0;
}

int Memory::getRefCount()
{
    return m_ref_counts;
}

char* Memory::getStart()
{
    return m_start;
}

char* Memory::getEnd()
{
    return m_end;
}

char* Memory::getBlock()
{
    int index = -1;
    {
        MutexLockGuard mutexguard(m_mutex);
        for(size_t i = 0;i<m_blocks.size();i++)
        {
            if(m_blocks[i] == false)
            {
                m_blocks[i] =true;
                index = i;
                break;
            }
        }
    }
    if(index == -1)
    {
        std::cout<<"no empty block"<<std::endl;
        return nullptr;
    }
    m_ref_counts++;
    return m_start + (index*m_block_size);
}

void Memory::backBlock(char* s)
{
    if(s > m_end || s<m_start)
    {
        std::cout<<"error,this block is not belong to this Memory"<<std::endl;
        return ;
    }

    {
        MutexLockGuard mutexguard(m_mutex);
        int index = (s-m_start)/m_block_size;
        m_blocks[index] = true;
        m_ref_counts--;
    }
}

bool Memory::hasBlock(char* s)
{
    return ((s >= m_start) && (s <= m_end));
}