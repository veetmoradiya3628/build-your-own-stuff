#include <iostream>
#include <fstream>
using namespace std;
int main()
{
    ifstream file;
    file.open("veet.txt");

    if (!file.is_open())
    {
        cout << "error while opening the file" << endl;
    }
    else
    {
        // read or write to the file
        string line;

        while (file.good())
        {
            getline(file, line);
            cout << line << endl;
        }
        file.close();
    }

    return 0;
}