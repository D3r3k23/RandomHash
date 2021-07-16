#ifndef RANDOM_HASH_HPP
#define RANDOM_HASH_HPP

#include "Util.hpp"

#include <vector>
#include <concepts>
#include <functional>
#include <string>

using uint = unsigned int;

const uint DEFAULT_SIZE = 23;
const float MAX_LOAD_FACTOR = 0.75;

namespace RH
{
    template <typename T>
    concept Hashable = requires(T x)
    {
        { std::hash<T>{}(x) } -> std::convertible_to<std::size_t>;
    };

    template <Hashable K, typename V>
    class RandomHash
    {
    private:
        template <typename K, typename V>
        struct Record
        {
            enum class State
            {
                EMPTY,
                ACTIVE,
                DELETED
            };

        K key;
        V val;
        State state;
    };

        std::vector<Record<K, V>> m_table;
        uint m_count;
        uint m_fullBuckets;

    public:
        RandomHash(uint capacity=DEFAULT_SIZE);

        void insert(const K& key, const V& val=V());
        void remove(const K& key);

        V& operator[](const K& key);
        const V& operator[](const K& key) const;

        size_t size(void) const;
        bool empty(void) const;
        bool contains(const K& key) const;

        uint bucket_count(void) const;
        float load_factor(void) const;

        std::string to_string(void) const;

    private:
        uint hash(const K& key) const;
        void rehash(void);
    };
}

#endif // RANDOM_HASH_HPP
