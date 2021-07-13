#include "RandomHash.hpp"

#include <iostream>

int main(int, char**)
{
    int prime = 0;
    for (int i = 0; i < 100; i++)
    {
        prime = Util::next_prime(prime);
        std::cout << prime << '\n';
    }

    return 0;
}