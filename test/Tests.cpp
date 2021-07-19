#include "Tests.hpp"

#include "RandomHash.hpp"

#include <iostream>
#include <chrono>
#include <random>

namespace RH::Tests
{
    void run_test(std::function<bool()> func, const char* name)
    {
        using Clock = std::chrono::steady_clock;
        using Time  = std::chrono::time_point<Clock>;
        using Micro = std::chrono::microseconds;

        std::cout << std::format("Test: {}", name) << '\n';

        Time start  = Clock::now();
        bool passed = func();
        Time end    = Clock::now();

        auto elapsed = std::chrono::duration_cast<Micro>(end - start).count();

        std::cout << std::format("Result: {}", passed ? "Passed" : "Failed") << '\n';
        std::cout << std::format("Profile: {} us", elapsed) << '\n';
        std::cout << '\n';
    }

    bool test1(void)
    {
        RH::RandomHash<std::string, int> h;
        h["Derek"] = 23;
        h["Geralt"] = 178;

        std::cout << h.to_string() << '\n';

        if  (h.size() != 2) return false;
        else if (h.empty()) return false;
        else return true;
    }

    bool test_hash(void)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distr(0, 9999);

        RH::RandomHash<int, int> h;
        for (int i = 0; i < 25; i++)
        {
            h[i*3] = distr(gen);
            std::cout << h.to_string() << '\n';
        }
        return true;
    }

    bool test_clear(void)
    {

        RH::RandomHash<int, int> h;
        for (int i = 0; i < 50; i++)
            h[i] = i;
        
        std::cout << "Size: " << h.size() << '\n';
        std::cout << "Buckets: " << h.bucket_count() << '\n';
        std::cout << "LF: " << h.load_factor() << '\n';

        std::cout << "Clearing table" << '\n';
        h.clear();
        std::cout << "Size: " << h.size() << '\n';
        std::cout << "Buckets: " << h.bucket_count() << '\n';
        std::cout << "LF: " << h.load_factor() << '\n';
        return h.empty();
    }

    bool test_remove(void)
    {
        return true;
    }
}
