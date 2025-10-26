//
// Created by pravinkumar on 10/4/25.
//

#include <algorithm>
#include <coroutine>
#include <iostream>

template <typename Generator, typename T>
class promise_type_base
{
    using CoroHandle = std::coroutine_handle<promise_type_base<Generator, T>>;
    T current_value;

public:
    Generator get_return_object() { return Generator{CoroHandle::from_promise(*this)}; }
    [[nodiscard]] std::suspend_always initial_suspend() const noexcept { return {}; }
    [[nodiscard]] std::suspend_always final_suspend() const noexcept { return {}; }

    const T& get_value()
    {
        return current_value;
    }

    std::suspend_always yield_value(T value)
    {
        current_value = std::move(value);
        return {};
    }

    void return_void() const noexcept
    {
    }

    void unhandled_exception() const noexcept { std::terminate(); }
};

template <typename CoroHandle, typename T>
class Iterator
{
    CoroHandle handle_;

public:
    explicit Iterator(CoroHandle handle) : handle_(handle)
    {
    }

    // Iterator& operator++()
    // {
    //     //if (!handle_.done())
    //     handle_.resume();
    //     if (handle_.done())
    //     {
    //         handle_ = nullptr;
    //     }
    //     return *this;
    // }
    void operator++()
    {
        handle_.resume();
    }

    bool operator==(std::default_sentinel_t) const
    {
        return !handle_ || handle_.done();
    }

    const T& operator*()
    {
        return handle_.promise().get_value();
    }

    bool operator==(const Iterator& other)
    {
        return handle_ == other.handle_;
    }

    bool operator!=(const Iterator& other)
    {
        return !(*this == other);
    }
};

template <typename T>
class Generator
{
public:
    using promise_type = promise_type_base<Generator, T>;
    using CoroHandle = std::coroutine_handle<promise_type>;

private:
    // handle
    CoroHandle handle_;

public:
    explicit Generator(CoroHandle handle) : handle_(handle)
    {
    }

    Generator() = delete;

    ~Generator()
    {
        if (handle_) handle_.destroy();
    }

    Generator(const Generator&) = delete;
    Generator& operator=(const Generator&) = delete;

    Generator(Generator&& other)
        noexcept
    {
        handle_ = other.handle_;
        other.handle_ = {};
    }

    Generator& operator=(Generator&& other)
        noexcept
    {
        if (this != &other)
        {
            if (handle_) handle_.destroy();
            handle_ = other.handle_;
            other.handle_ = {};
        }
        return *this;
    }

    Iterator<CoroHandle, T> begin()
    {
        if (handle_)
        {
            handle_.resume();
            if (handle_.done())
            {
                return Iterator<CoroHandle, T>{nullptr};
            }
        }
        return Iterator<CoroHandle, T>{handle_};
    }

    auto end()
    {
        //return Iterator<CoroHandle, T>{nullptr};
        return std::default_sentinel_t{};
    }
};

template <typename T>
Generator<T> generator_(T start, T end)
{
    while (start < end)
        co_yield start++;
}


int main()
{
    auto generator = generator_<int>(5, 10);
    for (auto&& value : generator)
    {
        std::cout << value << std::endl;
    }
}
