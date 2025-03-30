#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
using namespace std;

int myAmount = 0;
std::timed_mutex m;

void increament(int i)
{
    // timed mutex will wait till provided time for getting lock
    // try_lock_until - similar functionality functino but supports reference time not absolute time
    if (m.try_lock_for(std::chrono::seconds(1)))
    {
        ++myAmount;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        cout << "Thread " << i << " Entered" << endl;
        m.unlock();
    }
    else
    {
        cout << "Thread " << i << " Couldn't Entered" << endl;
    }
}

int main()
{
    std::thread t1(increament, 1);
    std::thread t2(increament, 2);

    t1.join();
    t2.join();

    cout << myAmount << endl;

    return 0;
}