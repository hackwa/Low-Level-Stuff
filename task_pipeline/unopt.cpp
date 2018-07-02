#include <iostream>
#include <thread>
#include <deque>
#include <future>
#include <functional>
#include <thread>
#include <mutex>
#include <memory>

using namespace std;
typedef unsigned long long _llu;
std::mutex mu;
std::condition_variable cond;

std::deque < std::packaged_task < _llu() > > task_q;

_llu _fib(int i)
{
    if (i < 3) return i - 1;
    return _fib(i-1) + _fib(i-2);
}

_llu fib (int i)
{
    _llu res = _fib(i);
    cout << "FIB " << i << " : " << res << endl << flush;
    return res;
}

void consumer ()
{
    packaged_task< _llu() > t;
    thread::id this_id = this_thread::get_id();
    while (1) {
        {
            std::unique_lock<std::mutex> locker(mu);
            cond.wait(locker, []() {return !task_q.empty();});
            cout << "TID " << this_id << endl << flush;
            t = std::move(task_q.front());
            task_q.pop_front();
            locker.unlock();
        }
        t();
    }
}

int main()
{
    unique_ptr<thread> t1[20];
    int cores = thread::hardware_concurrency()  - 2;
    int i;
    cout << "no of cores :" << cores << endl;
    for (i = 0; i < cores; i++)
        t1[i] = make_unique<std::thread>(consumer);
    for (i = 0; i < 100; i++) {
        {
            lock_guard<std::mutex> locker(mu);
            task_q.push_back( move ( packaged_task<_llu()>(bind(fib,i)) ));
        }
        cond.notify_one();
    }
    for (i = 0; i < cores; i++) {
        t1[i]->join();
    }
    return 0;
}