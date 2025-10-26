//
// Created by pravinkumar on 10/8/25.
//

#include <iostream>
#include <ostream>
#include <utility>

template <typename T>
class MyUniquePointer
{
    T* ptr;

public:
    MyUniquePointer() = delete;
    MyUniquePointer(const MyUniquePointer&) = delete;
    MyUniquePointer& operator=(const MyUniquePointer&) = delete;

    explicit MyUniquePointer(T* ptr) : ptr{ptr}
    {
    }

    MyUniquePointer(MyUniquePointer&& other)
        : ptr{other.ptr}
    {
        other.ptr = nullptr;
    }

    MyUniquePointer& operator=(MyUniquePointer&& other)
    {
        if (this != &other)
        {
            this->ptr = other.ptr;
            other.ptr = nullptr;
        }
    }

    ~MyUniquePointer()
    {
        if (ptr)
        {
            delete ptr;
        }
    }

    T* operator->()
    {
        return ptr;
    }

    T& operator*()
    {
        return *ptr;
    }

    T* get()
    {
        return ptr;
    }
};

template <typename T, typename... Args>
MyUniquePointer<T> make_my_unique_ptr(Args&&... args)
{
    return MyUniquePointer<T>(new T(std::forward<Args>(args)...));
}

int main()
{
    auto ptr = make_my_unique_ptr<int>(42);
    std::cout << *ptr << std::endl;
    // auto wontwork = ptr; // won't work
    auto another_ptr = std::move(ptr);
    std::cout << *another_ptr << std::endl;
    std::cout << "Accessing raw pointer << " << another_ptr.get() << std::endl;
}
