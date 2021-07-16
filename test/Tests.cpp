#include "Tests.hpp"

#include "RandomHash.hpp"

#include <iostream>
#include <chrono>

namespace RH::Tests
{
    void run_test(std::function<bool()> func, const char* name)
    {
        using Clock = std::chrono::steady_clock;
        using Time  = std::chrono::time_point<Clock>;
        using Micro = std::chrono::microseconds;

        Time start  = Clock::now();
        bool passed = func();
        Time end    = Clock::now();

        Micro elapsed = std::chrono::duration_cast<Micro>(end - start);

        std::cout << std::format("Test:    {}", name)                         << '\n';
        std::cout << std::format("Result:  {}", passed ? "Passed" : "Failed") << '\n';
        std::cout << std::format("Profile: {}", elapsed.count())              << '\n';
        std::cout << '\n';
    }

    bool test1(void)
    {
        RH::RandomHash<std::string, int> h;
        h["Derek"] = 23;
        h["Geralt"] = 178;

        std::cout << h.to_string() << '\n';

        if (h.size() != 2) return false;
        if (h.empty()) return false;
        return true;
    }
}
