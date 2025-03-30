#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

int count = 0;
std::mutex mtx;

void increaseTheCounterFor100000Time()
{
    for (int i = 0; i < 100000; i++)
    {
        // if not getting the lock then it will skip that execution

        // if (mtx.try_lock())
        // {
        //     ++count;
        //     mtx.unlock();
        // }

        // execution will produce the same result every single time

        mtx.lock();
        ++count;
        mtx.unlock();
    }
}

int main()
{
    std::thread t1(increaseTheCounterFor100000Time);
    std::thread t2(increaseTheCounterFor100000Time);
    t1.join();
    t2.join();
    cout << "counter increased upto : " << count << endl;
    return 0;
}