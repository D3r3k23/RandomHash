#include "Util.hpp"

#include <vector>

static std::vector<unsigned int> s_primes{
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41
};

namespace Util
{
    static

    unsigned int next_prime(unsigned int n)
    {
        while (!is_prime(n))
            n++;
        return n;
    }

    bool is_prime(unsigned int n)
    {
        if (n == 2 || n == 3)
            return true;
        if (n % 2 == 0 || n % 3 == 0)
            return false;

        for (int i = 0; i*i <= n; )
    }
}