#include <iostream>
#include <fstream>
using namespace std;

int main()
{
    // Your code here
    fstream file;
    file.open("veet.txt", ios_base::in | ios_base::out | ios_base::app);

    if (!file.is_open())
    {
        cout << "error while opening the file" << endl;
    }
    else
    {
        file << "Writing on file" << endl;
        file.seekg(0);

        string line;
        while (file.good())
        {
            getline(file, line);
            cout << line << endl;
        }
    }

    return 0;
}