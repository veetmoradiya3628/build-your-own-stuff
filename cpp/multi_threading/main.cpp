#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
using namespace std;

mutex mtx;

void hello()
{
    cout << "Hello from thread !" << endl;
}

void print(int x, const string &msg)
{
    cout << msg << " : " << x << endl;
}

void task(int id)
{
    cout << "Thread " << id << " is started" << endl;
    this_thread::sleep_for(chrono::milliseconds(200));
    cout << "Thread " << id << " is completed" << endl;
}

int main()
{
    thread t(hello);
    t.join();

    int num_of_threads = 200;
    vector<thread> threads;
    for (int i = 0; i < num_of_threads; i++)
    {
        threads.emplace_back(task, i);
    }
    for (auto &th : threads)
    {
        th.join();
    }
    cout << "All threads finished!" << endl;
    return 0;
}