#include <iostream>
using namespace std;
class Student
{
private:
    int age;

public:
    Student() : age(12) {}

    void get_age()
    {
        cout << "Age = " << age << endl;
    }
};
int main()
{
    Student *ptr = new Student();
    ptr->get_age();
    delete ptr;
    return 0;
}