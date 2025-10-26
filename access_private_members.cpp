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
    MySecretClass() : my_secret_variable(10) //, my_another_secret_variable("my string")
    {
    }

private:
    int my_secret_variable;
    //static std::string my_another_secret_variable;
};
int* get_data();
//static std::string* get_string_data();

template <int*x>
struct IntHacker
{
    friend int* get_data()
    {
        return x;
    };
};
//
// template <std::string*x>
// struct StringHacker
// {
//     friend std::string* get_string_data()
//     {
//         return x;
//     }
// };

template struct IntHacker<&MySecretClass::my_secret_variable>;
//template struct StringHacker<&MySecretClass::my_another_secret_variable>;

int main()
{
    int x = *get_data();
    // std::string s = *get_string_data();
    std::cout << "printing data" << std::endl;
    // std::cout << s << std::endl;
    std::cout << x << std::endl;
    return 0;
}



