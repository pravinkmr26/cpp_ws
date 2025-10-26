//
// Created by pravinkumar on 10/6/25.
//
#include <coroutine>
#include <exception>
#include <iostream>
#include <thread>

template <typename Task, typename T>
class promise_type_base
{
    using CoroHandle = std::coroutine_handle<typename Task::promise_type>;
    T result;
public:
    std::exception_ptr current_exception;
    std::suspend_never initial_suspend() const noexcept { return {}; }
    std::suspend_always final_suspend() const noexcept { return {}; }

    void unhandled_exception() noexcept
    {
        current_exception = std::current_exception();
    }

    void return_value(std::string value)
    {
        result = std::move(value);
    }

    Task get_return_object() noexcept { return Task{CoroHandle::from_promise(*this)}; }
};

class awaitable_task
{
    std::string my_bid_data{};
public:
    bool await_ready() const noexcept { return false; }

    std::string await_resume() const noexcept
    {
        return my_bid_data;
    }

    // this can return void/bool or the coro_handle itself. Read more on this
    void await_suspend(std::coroutine_handle<> coro_handle) noexcept
    {
        std::jthread([handle=coro_handle, this]()
        {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            //handle.promise().set_value("my really long long data is ready");
            this->my_bid_data = "my bigggie data";
            handle.resume();
        }).detach();
    }
};


template <typename T>
class Task
{
public:
    using promise_type = promise_type_base<Task, T>;
    using CoroHandle = std::coroutine_handle<promise_type>;

    explicit Task(CoroHandle coroutine_handle): coroutine_handle_(coroutine_handle)
    {
    }

    ~Task()
    {
        if (coroutine_handle_) coroutine_handle_.destroy();
    }
private:
    CoroHandle coroutine_handle_;
};

Task<std::string> get_data()
{
    std::cout << "Going to read my really big data" << std::endl;

    std::string data = co_await awaitable_task{};
    std::cout << "Read my really big data: " << data << std::endl;
}

int main()
{
    auto task = get_data();
    //task.resume();
    std::cout << "Main thread sleeps!" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(6));
    std::cout << "Main thread woke up!" << std::endl;

    return 0;
}
