#include <iostream>
using namespace std;

int main()
{
    // reads single line
    string data;
    // cin >> data;
    // cout << data;

    // reads entire line and put it in data variable
    getline(cin, data);
    cout << data << endl;
    return 0;
}