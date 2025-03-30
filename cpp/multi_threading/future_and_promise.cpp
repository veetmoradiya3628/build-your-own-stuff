/**

1. std::promise
- used to set values or exceptions

2. std::future
- used to get values from promise
- ask promise if the value is available
- wait for the promise
 */

#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>
#include <future>
using namespace std;
using namespace std::chrono;
typedef long int ull;

void findOdd(std::promise<ull> &&OddSumPromise, ull start, ull end)
{
    ull OddSum = 0;
    for (ull i = start; i <= end; i++)
    {
        if (i & 1)
        {
            OddSum += i;
        }
    }
    OddSumPromise.set_value(OddSum);
}

int main()
{
    ull start = 0, end = 1900000000;
    std::promise<ull> OddSum;
    std::future<ull> OddFuture = OddSum.get_future();
    cout << "Thread created!" << endl;
    std::thread t1(findOdd, std::move(OddSum), start, end);
    cout << "Waiting for Result!!" << endl;
    cout << "OddSum : " << OddFuture.get() << endl;
    cout << "Completed!!" << endl;
    t1.join();
    return 0;
}