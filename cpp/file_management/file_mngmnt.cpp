#include <iostream>
#include <fstream>
using namespace std;
// ifstream, ofstream, fstream
// in, out, trunk, app - append mode, ate - seek, binary
int main()
{
    fstream file;
    file.open("veet.txt", ios_base::in | ios_base::out | ios_base::trunc);

    if (!file.is_open())
    {
        cout << "error while opening the file" << endl;
    }
    else
    {
        // read or write to the file
        cout << "file opened successfully" << endl;
        file.close();
    }

    return 0;
}