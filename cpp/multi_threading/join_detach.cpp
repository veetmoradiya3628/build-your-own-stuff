#include <iostream>
#include <thread>
#include <chrono>
using namespace std;

void run(int count)
{
    while (count-- > 0)
    {
        cout << "CppNuts" << endl;
    }
    std::this_thread::sleep_for(chrono::seconds(5));
}

int main()
{
    thread t1(run, 10);
    cout << "main()" << endl;
    
    // t1.join();
    // if (t1.joinable())
    //     t1.join();
    
    t1.detach();
    
    cout << "main() after" << endl;
    return 0;
}