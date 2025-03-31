#include <iostream>
#include <fstream>
using namespace std;
int main()
{
    ofstream file;
    file.open("veet.txt");

    if (!file.is_open())
    {
        cout << "error while opening the file" << endl;
    }
    else
    {
        // read or write to the file
        file << "Leading File system" << endl;
        file << "This is 2nd Line" << endl;
        
        cout << "Successfully written to the file" << endl;
        file.close();
    }

    return 0;
}