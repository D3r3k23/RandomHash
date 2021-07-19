#include "Tests.hpp"

int main(int, char**)
{
    RUN_TEST(test1);
    RUN_TEST(test_hash);
    RUN_TEST(test_clear);

    return 0;
}
