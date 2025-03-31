#include <iostream>
#include <fstream>
using namespace std;

int main()
{
    ifstream file("veet.txt", ios::in);
    if (!file.is_open())
    {
        cout << "error" << endl;
    }
    else
    {
        // tellp & seekp (with offset / without offset)
        cout << file.tellg() << endl;
        string line;
        file.seekg(3);
        getline(file, line);
        cout << line << endl;
        cout << file.tellg() << endl;
    }
    return 0;
}