#ifndef RH_TESTS_HPP
#define RH_TESTS_HPP

#include "RandomHash.hpp"

#include <functional>

#define RUN_TEST(test) RH::Tests::run_test(RH::Tests::test, #test)

namespace RH::Tests
{
    void run_test(std::function<bool()> func, const char* name);

    bool test1(void);
    bool test_hash(void);
    bool test_clear(void);
    bool test_remove(void);
}

#endif // RH_TESTS_HPP
