#include "Util.hpp"

#include <vector>

namespace Util
{
    // static std::vector<unsigned int> s_primes{
    //     2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41
    // };

    unsigned int next_prime(unsigned int n)
    {
        while (!is_prime(n))
            n++;
        return n;
    }

    bool is_prime(unsigned int n)
    {
        if (n < 2)
            return false;
        if (n == 2)
            return true;
        if (n % 2 == 0)
            return false;

        for (int i = 3; i*i <= n; i += 2)
            if (n % i == 0)
                return false;
        
        return true;
    }
}
