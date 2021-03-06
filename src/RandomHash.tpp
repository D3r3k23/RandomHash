#ifndef RH_RANDOM_HASH_TPP
#define RH_RANDOM_HASH_TPP

#include "RandomHash.hpp"

#include <format>
#include <sstream>

namespace RH
{
    template <Hashable K, typename V>
    RandomHash<K, V>::RandomHash(uint initCapacity)
      : m_table(next_prime(initCapacity)),
        m_INIT_CAPACITY(initCapacity),
        m_count(0),
        m_fullBuckets(0)
    {
        for (auto& entry : m_table)
            entry.state = RecordState::EMPTY;
    }

    template <Hashable K, typename V>
    void RandomHash<K, V>::insert(const K& key, const V& val)
    {
        auto& entry = m_table[hash(key)];

        if (entry.state == RecordState::ACTIVE)
            throw KeyError(std::format("Key: {} already exists", key));
        else
        {
            if (entry.state == RecordState::DELETED) // entry.key == key
            {
                entry.state = RecordState::ACTIVE;
                if (entry.val != val)
                    entry.val = val;
            }
            else // entry.state == RecordState::EMPTY
            {
                entry = { key, val, RecordState::ACTIVE };
                m_fullBuckets++;
            }
            m_count++;

            if (load_factor() > MAX_LOAD_FACTOR)
                rehash();
        }
    }

    template <Hashable K, typename V>
    void RandomHash<K, V>::remove(const K& key)
    {
        auto& entry = m_table[hash(key)];

        if (entry.state != RecordState::ACTIVE)
            throw KeyError(std::format("Key: {} does not exist", key));
        else
        {
            entry.state = RecordState::DELETED;
            m_count--;
        }
    }

    template <Hashable K, typename V>
    void RandomHash<K, V>::clear(void)
    {
        m_table.clear();
        m_table.resize(m_INIT_CAPACITY);
        m_count = 0;
        m_fullBuckets = 0;
    }

    template <Hashable K, typename V>
    V& RandomHash<K, V>::operator[](const K& key)
    {
        auto& entry = m_table[hash(key)];

        if (entry.state != RecordState::ACTIVE)
            insert(key);

        return entry.val;
    }

    template <Hashable K, typename V>
    const V& RandomHash<K, V>::operator[](const K& key) const
    {
        const auto& entry = m_table[hash(key)];

        if (entry.state != RecordState::ACTIVE)
            throw KeyError(std::format("Key: {} does not exist", key));
        else
            return entry.val;
    }

    template <Hashable K, typename V>
    uint RandomHash<K, V>::size(void) const
    {
        return m_count;
    }

    template <Hashable K, typename V>
    bool RandomHash<K, V>::empty(void) const
    {
        return m_count == 0;
    }

    template <Hashable K, typename V>
    bool RandomHash<K, V>::contains(const K& key) const
    {
        return m_table[hash(key)].state == RecordState::ACTIVE;
    }

    template <Hashable K, typename V>
    uint RandomHash<K, V>::bucket_count(void) const
    {
        return m_table.size();
    }

    template <Hashable K, typename V>
    float RandomHash<K, V>::load_factor(void) const
    {
        return static_cast<float>(m_fullBuckets) / static_cast<float>(bucket_count());
    }

    template <Hashable K, typename V>
    std::string RandomHash<K, V>::to_string(void) const
    {
        std::stringstream ss;
        ss << "{\n";

        for (const auto& entry : m_table)
            if (entry.state == RecordState::ACTIVE)
                ss << std::format("  {}: {}\n", entry.key, entry.val);

        ss << "}\n";
        return ss.str();
    }

    template <Hashable K, typename V>
    uint RandomHash<K, V>::hash(const K& key) const
    {
        uint hashVal = std::hash<K>{}(key);

        for (uint i = 0; i < bucket_count(); i++)
        {
            uint index = (hashVal + i) % bucket_count();
            const auto& entry = m_table[index];

            if (entry.state == RecordState::EMPTY || entry.key == key)
                return index;
        }

        // No bucket found
        throw HashError(std::format("No bucket found for key: {}", key));
    }

    template <Hashable K, typename V>
    void RandomHash<K, V>::rehash(void)
    {
        uint newBucketCount = next_prime(2 * bucket_count());

        RH_LOG(std::format("Rehashing: {} -> {}", bucket_count(), newBucketCount));

        auto oldTable = std::move(m_table);
        clear();
        m_table.resize(newBucketCount);

        for (const auto& entry : oldTable)
            if (entry.state == RecordState::ACTIVE)
                insert(entry.key, entry.val);
    }

    // KeyIterator

    template <Hashable K, typename V>
    KeyIterator<K, V>::KeyIterator(const RecordIterator<K, V>& it)
        : it(it)
    { }

    template <Hashable K, typename V>
    const K& KeyIterator<K, V>::operator*(void) const
    {
        return it->key;
    }

    template <Hashable K, typename V>
    const K& KeyIterator<K, V>::operator->(void) const
    {
        return it->key;
    }

    template <Hashable K, typename V>
    KeyIterator<K, V>::operator bool(void) const
    {
        return it->state == RecordState::ACTIVE;
    }

    template <Hashable K, typename V>
    bool KeyIterator<K, V>::operator==(const KeyIterator& other) const
    {
        return *it == *(other.it);
    }

    template <Hashable K, typename V>
    bool KeyIterator<K, V>::operator!=(const KeyIterator& other) const
    {
        return !(*this == other);
    }

    template <Hashable K, typename V>
    KeyIterator<K, V>& KeyIterator<K, V>::operator++(void)
    {
        return ++it;
    }

    template <Hashable K, typename V>
    KeyIterator<K, V> KeyIterator<K, V>::operator++(int)
    {
        return it++;
    }

    template <Hashable K, typename V>
    KeyIterator<K, V>& KeyIterator<K, V>::operator--(void)
    {
        return --it;
    }

    template <Hashable K, typename V>
    KeyIterator<K, V> KeyIterator<K, V>::operator--(int)
    {
        return it--;
    }
}

#endif // RH_RANDOM_HASH_TPP
