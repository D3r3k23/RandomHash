#ifndef UTIL_HPP
#define UTIL_HPP

#ifdef RH_DEBUG
    #include <iostream>
    #define RH_LOG(msg) do { std::cout << msg << '\n'; } while (false)
#else
    #define RH_LOG
#endif

namespace Util
{
    unsigned int next_prime(unsigned int n);
    bool is_prime(unsigned int n);
}

#endif // UTIL_HPP
