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

        template <Hashable K, typename V> friend class KeyIterator;

        KeyIterator<K, V> begin(void) { return KeyIterator<K, V>(m_table.begin()); }
        KeyIterator<K, V> end(void)   { return KeyIterator<K, V>(m_table.end()); }

    private:
        uint hash(const K& key) const;
        void rehash(void);
    };

    template <Hashable K, typename V>
    using RecordIterator = std::vector<Record<K, V>>::iterator;

    template <Hashable K, typename V>
    class KeyIterator
    {
    private:
        RecordIterator<K, V> it;

    public:
        KeyIterator(void) = delete;
        KeyIterator(const RecordIterator<K, V>& it);

        KeyIterator(const KeyIterator& other) = default;
        KeyIterator& operator=(const KeyIterator& other) = default;

        const K& operator*(void)  const;
        const K& operator->(void) const;

        operator bool(void) const;

        bool operator==(const KeyIterator& other) const;
        bool operator!=(const KeyIterator& other) const;

        KeyIterator& operator++(void);
        KeyIterator  operator++(int);

        KeyIterator& operator--(void);
        KeyIterator  operator--(int);
    };

    struct KeyError : public std::runtime_error
    {
        KeyError(const std::string& what) : std::runtime_error("[RH::KeyError] " + what) {}
    };

    struct HashError : public std::runtime_error
    {
        HashError(const std::string& what) : std::runtime_error("[RH::HashError] " + what) {}
    };
}

#include "RandomHash.tpp" // Template definitions

#endif // RH_RANDOM_HASH_HPP
