#include <iostream>
using namespace std;

int main()
{
    int *ptr_int;
    cout << ptr_int << endl;

    ptr_int = new int;
    *ptr_int = 45;
    cout << *ptr_int << endl;

    float *ptr_flt = new float{23.6};
    cout << ptr_flt << endl;
    cout << *ptr_flt << endl;

    delete ptr_int;
    delete ptr_flt;
    return 0;
}