//
// Created by pravinkumar on 10/7/25.
//
#include <coroutine>
#include <exception>
#include <iostream>
#include <string>
#include <thread>

template <typename T>
class Task
{
public:
    class promise_type_base
    {
    public:
        Task get_return_object()
        {
            return Task{Task::CoroHandle::from_promise(*this)};
        }

        std::suspend_never initial_suspend() const noexcept
        {
            return {};
        }

        std::suspend_never final_suspend() const noexcept
        {
            return {};
        }

        // void return_value(T value)
        // {
        //     value_ = value;
        // }
        void return_value()
        {
        }

        void unhandled_exception()
        {
            exception_ = std::current_exception();
        }

    private:
        //T value_;
        std::exception_ptr exception_;
    };

    using promise_type = promise_type_base;
    using CoroHandle = std::coroutine_handle<promise_type>;

    explicit Task(CoroHandle handle) : handle_(handle)
    {
    }

    ~Task()
    {
        if (!handle_.done()) handle_.destroy();
    }

private:
    CoroHandle handle_;
};


class AwaitableCoroutine
{
    std::string a_value{};

public:
    bool await_ready()
    {
        return false;
    }

    void await_suspend(std::coroutine_handle<> handle) noexcept
    {
        std::jthread([coroutine_handle = handle, this]()
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            this->a_value = "my value";
            coroutine_handle.resume();
        }).detach();
    }

    std::string await_resume() const noexcept
    {
        return a_value;
    }
};

Task<std::string> the_super_long_task()
{
    std::cout << "My super long task is starting up!" << std::endl;
    auto value = co_await AwaitableCoroutine{};
    std::cout << "My super long task has finished with a result: " << value << std::endl;
}

int main()
{
    std::cout << "Main thread starting!" << std::endl;
    std::cout << "Calling my super long task " << std::endl;
    Task<std::string> task = the_super_long_task();
    std::cout << "Main thread sleeping!" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout << "Main thread woke up!" << std::endl;
    std::cout << "Done!" << std::endl;
    return 0;
}
