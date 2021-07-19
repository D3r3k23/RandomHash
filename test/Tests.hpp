#ifndef TESTS_HPP
#define TESTS_HPP

#include <functional>

#define RUN_TEST(test) RH::Tests::run_test(RH::Tests::test, #test)

namespace RH::Tests
{
    void run_test(std::function<bool()> func, const char* name);

    bool test1(void);
    bool test_clear(void);
    bool test_remove(void);
}

#endif // TESTS_HPP
