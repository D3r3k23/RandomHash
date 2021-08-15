#include "Prime.hpp"

namespace RH
{   
    uint next_prime(uint n)
    {
        while (!is_prime(n))
            n++;
        return n;
    }

    bool is_prime(uint n)
    {
        if (n < 2)
            return false;
        if (n == 2)
            return true;
        if (n % 2 == 0)
            return false;

        for (uint i = 3; i*i <= n; i += 2)
            if (n % i == 0)
                return false;
        
        return true;
    }
}
