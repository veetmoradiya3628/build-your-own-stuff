#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

int main()
{
    char input[100];
    strcpy(input, "Learning cpp file system");

    fstream file("veet.bin", ios_base::binary | ios_base ::in | ios_base ::out | ios::trunc);
    if (!file.is_open())
    {
        cout << "error while opening file" << endl;
    }
    else
    {
        int length = strlen(input);
        for (int counter = 0; counter <= length; counter++)
        {
            file.put(input[counter]);
        }

        file.seekg(0);
        char ch;
        while (file.good())
        {
            file.get(ch);
            cout << ch;
        }
    }
    return 0;
}