#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

class Person
{
    char name[80];
    int age;

public:
    Person()
    {
        strcpy(name, "noname");
        age = 0;
    }

    Person(const char *name, int age)
    {
        strncpy(this->name, name, sizeof(this->name) - 1);
        this->name[sizeof(this->name) - 1] = '\0'; // Ensure null termination
        this->age = age;
    }

    void whoAreYou() const
    {
        cout << "Hi, I am " << name << " and I am " << age << " years old." << endl;
    }

    // Proper serialization
    void writeToFile(fstream &file)
    {
        file.write(name, sizeof(name));
        file.write(reinterpret_cast<char *>(&age), sizeof(age));
    }

    // Proper deserialization
    void readFromFile(fstream &file)
    {
        file.read(name, sizeof(name));
        file.read(reinterpret_cast<char *>(&age), sizeof(age));
    }
};

int main()
{
    Person veet("Veet", 24);

    fstream file("person.bin", ios::in | ios::out | ios::trunc | ios::binary);
    if (!file.is_open())
    {
        cout << "Error opening file!" << endl;
        return 1;
    }

    // Use proper serialization
    veet.writeToFile(file);
    file.seekg(0);

    Person test;
    test.readFromFile(file);

    veet.whoAreYou();
    test.whoAreYou();

    file.close();
    return 0;
}
