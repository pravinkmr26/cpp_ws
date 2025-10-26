//
// Created by pravinkumar on 10/8/25.
//
// https://www.geeksforgeeks.org/cpp/can-we-access-private-data-members-of-a-class-without-using-a-member-or-a-friend-function-in-cpp/
// https://stackoverflow.com/questions/70506997/accessing-private-member-values-with-external-template-function

#include <iostream>
#include <ostream>
#include <string>

class MySecretClass
{
public:
    MySecretClass() : my_secret_variable(10)
    {
    }

private:
    int my_secret_variable;
};
int* get_data();

template <int MySecretClass::*var>
struct IntHacker
{
    static int get_data(MySecretClass& obj)
    {
        return obj.*var;
    };
};

int main()
{
    MySecretClass obj;
    int value = IntHacker<&MySecretClass::my_secret_variable>::get_data(obj);
    std::cout << "printing data" << std::endl;
    std::cout << value << std::endl;
    return 0;
}



