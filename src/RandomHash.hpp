#ifndef RANDOM_HASH_HPP
#define RANDOM_HASH_HPP

#include "Record.hpp"
#include "Util.hpp"

#include <vector>
#include <string>

using uint = unsigned int;

const uint DEFAULT_SIZE = 23;

namespace RH
{
    template <typename K, typename V>
    class RandomHash
    {
    private:
        std::vector<Record<K, V>> m_table;
        uint m_size;

    public:
        RandomHash(uint capacity=DEFAULT_SIZE)
            : m_table(Util::next_prime(capacity)),
              m_size(0)
        { }

    private:

    };
    
    uint hash(int key);
    uint hash(std::string key);
}

#endif // RANDOM_HASH_HPP