#pragma once

#include <iostream>
#include <string>
#include "Thread.hpp"
#include <functional>
#include <vector>
#include <queue>
class ThreadPool
{
public:
    ThreadPool(int num = 5)
        : _num(num),_isruning(true)
    {
        pthread_mutex_init(&_mut,nullptr);
        pthread_cond_init(&_cod,nullptr);
    }
    void task()
    {
        while(true)
        {
            pthread_mutex_lock(&_mut);

            while(_task_queue.empty()&&_isruning)
            {
                pthread_cond_wait(&_cod,&_mut);
            }
            
            while(_task_queue.empty()&&!_isruning)
            {
                pthread_mutex_unlock(&_mut);
                break;
            }

            func_t t = _task_queue.front();
            _task_queue.pop();

            pthread_mutex_unlock(&_mut);
            t();

        }
    }

    void Equeue(const func_t &in)
    {
        pthread_mutex_unlock(&_mut);

        _task_queue.push(in);
        pthread_cond_signal(&_cod);
        pthread_mutex_unlock(&_mut);
    }

    void Start()
    {
        func_t t =std::bind(&ThreadPool::task,this);
        for(int i =0;i<_num ;i++)
        {
            std::string name= "thread-" + std::to_string(i+1);
            _threads.emplace_back(name,t);
        }
    }
private:
    std::queue<func_t> _task_queue;
    std::vector<Thread> _threads;
    int _num;
    pthread_mutex_t _mut;
    pthread_cond_t _cod;
    bool _isruning;
};