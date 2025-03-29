#include <iostream>
#include <chrono>
#include <thread>

using namespace std;
#define ull unsigned long long
ull odd_sum = 0, even_sum = 0;

void calc_odd_sum(ull start, ull end)
{
    for (ull i = start; i <= end; i++)
    {
        if (i % 2 != 0)
        {
            odd_sum += i;
        }
    }
}

void calc_even_sum(ull start, ull end)
{
    for (ull i = start; i <= end; i++)
    {
        if (i % 2 == 0)
        {
            even_sum += i;
        }
    }
}
int main()
{
    ull start = 0, end = 1900000000;
    auto startTime = chrono::high_resolution_clock::now();
    thread t1(calc_even_sum, start, end);
    thread t2(calc_odd_sum, start, end);

    t1.join();
    t2.join();
   
    // calc_even_sum(start, end);
    // calc_odd_sum(start, end);

    auto stopTime = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stopTime - startTime);
    cout << "Time : " << (duration.count() / 1000000.0) << endl;
    return 0;
}