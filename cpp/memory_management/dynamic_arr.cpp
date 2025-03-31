#include <iostream>
using namespace std;

int main()
{
    int num;
    cin >> num;
    int *ptr;
    ptr = new int[num];
    for (int i = 0; i < num; i++)
    {
        cin >> *(ptr + i);
    }

    for (int i = 0; i < num; i++)
    {
        cout << *(ptr + i) << " ";
    }
    cout << endl;

    delete[] ptr;
    ptr = nullptr;

    return 0;
}