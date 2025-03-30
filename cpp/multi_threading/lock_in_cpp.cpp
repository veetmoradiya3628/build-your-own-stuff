#include <iostream>
#include <thread>
#include <mutex>
using namespace std;
// it is used to lock multiple mutex at the same time.
// syntax --> std::lock(m1, m2, m3, m4 ...);
std::mutex m1, m2;
void taks_a()
{
    while (1)
    {
        std::lock(m1, m2);
        cout << "task a\n";
        m1.unlock();
        m2.unlock();
    }
}
void task_b()
{
    while (1)
    {
        std::lock(m2, m1);
        cout << "task b\n";
        m2.unlock();
        m1.unlock();
    }
}
int main()
{
    std::thread t1(taks_a);
    std::thread t2(task_b);
    t1.join();
    t2.join();
    return 0;
}