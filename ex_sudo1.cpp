#include <functional>
#include <iostream>
#include <ostream>
#include <utility>
#include <cassert>

template <typename MemberT, typename ClassT>
struct PointerType
{
    // pointer type to the member
    using type = MemberT ClassT::*;
    using value_type = MemberT;
};

template <typename PointerType, auto ...>
struct Storage
{
    inline static typename PointerType::type pointer_to_member {};
};

template <typename PointerType, typename PointerType::type member>
struct Storage<PointerType, member>
{
    const inline static typename PointerType::type pointer_to_member = Storage<PointerType>::pointer_to_member = member;
};

template<typename PointerType, typename ObjectT>
decltype(auto) get_value(ObjectT&& object)
{
    assert(Storage<PointerType>::pointer_to_member);
    return std::invoke(Storage<PointerType>::pointer_to_member, std::forward<ObjectT>(object));
}

template <typename PointerType, typename ObjectT>
void set_value(ObjectT&& object, typename PointerType::value_type value)
{
    assert(Storage<PointerType>::pointer_to_member);
    //object.*Storage<PointerType>::pointer_to_member = value;
    std::invoke(Storage<PointerType>::pointer_to_member, std::forward<ObjectT>(object)) = value;
}

class MySecretClass
{
    int my_secret_variable {19};
};

using PointerType_ = PointerType<int, MySecretClass>;
template struct Storage<PointerType_, &MySecretClass::my_secret_variable>;

//using storage_type = Storage<PointerType_, &MySecretClass::my_secret_variable>;

int main()
{
    MySecretClass obj;
    std::cout << get_value<PointerType_>(obj) << std::endl;
    set_value<PointerType_>(obj, 42);
    std::cout << get_value<PointerType_>(obj) << std::endl;
    set_value<PointerType_>(obj, 45);
    std::cout << get_value<PointerType_>(obj) << std::endl;
    return 0;
}



