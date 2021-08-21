#ifndef RH_RANDOM_HASH_HPP
#define RH_RANDOM_HASH_HPP

#include "Prime.hpp"

#include <vector>
#include <string>
#include <concepts>
#include <functional>
#include <stdexcept>

#ifdef RH_DEBUG
    #include <iostream>
    #define RH_LOG(msg) do { std::cout << msg << '\n'; } while (false)
#else
    #define RH_LOG
#endif

namespace RH
{
    using uint = unsigned int;

    const uint DEFAULT_CAPACITY = 11;
    const float MAX_LOAD_FACTOR = 0.75;

    template <typename T>
    concept Hashable = requires(T x)
    {
        { std::hash<T>{}(x) } -> std::convertible_to<std::size_t>;
    };

    enum class RecordState
    {
        EMPTY,
        ACTIVE,
        DELETED
    };

    template <Hashable K, typename V>
    struct Record
    {
        K key;
        V val;
        RecordState state;
    };

    template <Hashable K, typename V>
    class RandomHash
    {
    private:
        std::vector<Record<K, V>> m_table;
        const uint m_INIT_CAPACITY;
        uint m_count;
        uint m_fullBuckets;

    public:
        RandomHash(uint initCapacity=DEFAULT_CAPACITY);

        void insert(const K& key, const V& val=V());
        void remove(const K& key);

        void clear(void);

        V& operator[](const K& key);
        const V& operator[](const K& key) const;

        uint size(void) const;
        bool empty(void) const;
        bool contains(const K& key) const;

        uint bucket_count(void) const;
        float load_factor(void) const;

        std::string to_string(void) const;

    private:
        uint hash(const K& key) const;
        void rehash(void);
    };

    struct KeyError : public std::runtime_error
    {
        KeyError(const std::string& what) : std::runtime_error("[KeyError] " + what) {}
    };

    struct HashError : public std::runtime_error
    {
        HashError(const std::string& what) : std::runtime_error("[HashError] " + what) {}
    };
}

#include "RandomHash.tpp" // Template definitions

#endif // RH_RANDOM_HASH_HPP
