#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>
#include <future>
using namespace std;
using namespace std::chrono;
typedef long int ull;

ull findOdd(ull start, ull end)
{
    ull OddSum = 0;
    cout << "ThreadID of findOdd : " << std::this_thread::get_id() << endl;
    for (ull i = start; i <= end; i++)
    {
        if (i & 1)
        {
            OddSum += i;
        }
    }
    return OddSum;
}

int main()
{
    ull start = 0, end = 1900000000;
    cout << "ThreadID : " << std::this_thread::get_id() << endl;
    cout << "Thread created if policy is std::launch::async!!" << endl;
    std::future<ull> OddSum = std::async(std::launch::async, findOdd, start, end); // deferred
    cout << "Waiting for Result!" << endl;
    cout << "OddSum : " << OddSum.get() << endl;
    cout << "Completed!" << endl;
    return 0;
}