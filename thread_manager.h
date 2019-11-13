#ifndef THREAD_MANAGER_H
#define THREAD_MANAGER_H
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <functional>
#include <vector>
#include <queue>

template<typename T>
class threadsafe_queue
{
private:
    mutable std::mutex mut; // 1 互斥量必须是可变的
    std::queue<T> data_queue;
    std::condition_variable data_cond;
public:
    threadsafe_queue()
    {}
    threadsafe_queue(threadsafe_queue const& other)
    {
        std::lock_guard<std::mutex> lk(other.mut);
        data_queue=other.data_queue;
    }
    void push(T new_value)
    {
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(new_value);
        data_cond.notify_one();
    }
    void wait_and_pop(T& value)
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk,[this]{return !data_queue.empty();});
        value=data_queue.front();
        data_queue.pop();
    }
    std::shared_ptr<T> wait_and_pop()
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk,[this]{return !data_queue.empty();});
        std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
        data_queue.pop();
        return res;
    }
    bool try_pop(T& value)
    {
        std::lock_guard<std::mutex> lk(mut);
        if(data_queue.empty())
            return false;
        value=data_queue.front();
        data_queue.pop();
        return true;
    }
    std::shared_ptr<T> try_pop()
    {
        std::lock_guard<std::mutex> lk(mut);
        if(data_queue.empty())
            return std::shared_ptr<T>();
        std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
        data_queue.pop();
        return res;
    }
    bool empty() const
    {
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.empty();
    }
};

class join_threads
{
public:
    explicit join_threads(std::vector<std::thread>& threads_):threads(threads_)
    {}
    ~join_threads()
    {
        for(unsigned long i=0;i<threads.size();++i)
        {
            if(threads[i].joinable())
                threads[i].join();
        }
    }
private:
    std::vector<std::thread>& threads;
};

class CThreadTask {
protected:
    CThreadTask();
public:
    virtual ~CThreadTask();
public:
    virtual void run() = 0;

};

class thread_pool
{
public:
    thread_pool(unsigned int thread_count = std::thread::hardware_concurrency()): m_thread_count(thread_count), done(false),joiner(threads)
    {

    }
    ~thread_pool()
    {
        done=true; // 11
    }

    void start()
    {
        try
        {
            for(unsigned i=0;i<m_thread_count;++i)
            {
                threads.push_back(std::thread(&thread_pool::worker_thread,this)); // 9
            }
        }
        catch(...)
        {
            done=true; // 10
            throw;
        }
    }
    void submit(CThreadTask *task)
    {
        work_queue.push(task); // 12
    }
private:
    unsigned int m_thread_count;
    std::atomic_bool done;
    threadsafe_queue<CThreadTask *> work_queue; // 1
    std::vector<std::thread> threads; // 2
    join_threads joiner; // 3
    void worker_thread()
    {
        while(!done) // 4
        {
           CThreadTask *task = nullptr;
           if(work_queue.try_pop(task)) // 5
           {
               if (task) {
                   task->run(); // 6
                   delete task;
               }
           }
           else
           {
               std::this_thread::yield(); // 7
           }
        }
    }
};



#endif // THREAD_MANAGER_H
