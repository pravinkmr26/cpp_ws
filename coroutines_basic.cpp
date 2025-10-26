//
// Created by pravinkumar on 10/4/25.
//


#include <coroutine>
#include <iostream>

struct Generator {
    struct promise_type {
        int current_value;
        Generator get_return_object() { return Generator{ std::coroutine_handle<promise_type>::from_promise(*this) }; }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(int value) {
            current_value = value;
            return {};
        }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };

    std::coroutine_handle<promise_type> handle;

    explicit Generator(std::coroutine_handle<promise_type> h) : handle(h) {}

    ~Generator() {
        if (handle) handle.destroy();
    }

    bool next() {
        if (!handle.done()) handle.resume();
        return !handle.done();
    }

    int value() const {
        return handle.promise().current_value;
    }
};

Generator counter() {
    for (int i = 0; i < 3; ++i)
        co_yield i;
}

int main() {
    auto gen = counter();
    // while (gen.next()) {
    //     std::cout << gen.value() << std::endl;
    // }
    for (auto && i : gen)
    {
        std::cout << i.value() << std::endl;
    }
}
