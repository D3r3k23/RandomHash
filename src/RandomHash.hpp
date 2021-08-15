#ifndef RANDOM_HASH_HPP
#define RANDOM_HASH_HPP

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

    uint next_prime(uint n);
    bool is_prime(uint n);

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
        KeyError(const char* what)        : std::runtime_error(what) {}
        KeyError(const std::string& what) : std::runtime_error(what) {}
    };

    struct HashError : public std::runtime_error
    {
        HashError(const char* what)        : std::runtime_error(what) {}
        HashError(const std::string& what) : std::runtime_error(what) {}
    };
}

#include "RandomHash.cpp" // Template definitions

#endif // RANDOM_HASH_HPP
