#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

std::mutex m;
long long bankBalance = 0;

void addMoney(long long val)
{
    m.lock();
    bankBalance += val;
    m.unlock();
    // std::this_thread::sleep_for(std::chrono::seconds(1));
}

int main()
{
    std::thread t1(addMoney, 100);
    std::thread t2(addMoney, 200);
    t1.join();
    t2.join();
    cout << "Final BankBalance : " << bankBalance << endl;
    return 0;
}