#include "RandomHash.hpp"

#include <format>
#include <stdexcept>

namespace RH
{
    template <Hashable K, typename V>
    RandomHash<K, V>::RandomHash(uint capacity)
        : m_table(Util::next_prime(capacity)),
          m_count(0), m_fullBuckets(0)
    {
        for (auto& entry : m_table)
            entry.state = Record::State::EMPTY;
    }

    template <Hashable K, typename V>
    void RandomHash<K, V>::insert(const K& key, const V& val)
    {
        auto& entry = m_table(hash(key));

        if (entry.state == Record::State::ACTIVE)
            throw std::out_of_range(std::format("Key: {} already exists", key));
        else
        {
            if (entry.state == Record::State::DELETED)
            {
                entry.state = Record::State::ACTIVE;
                if (entry.val != val)
                    entry.val = val;
            }
            else if (entry.state == Record::State::EMPTY)
            {
                entry = {key, val, Record::State::ACTIVE};
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
        auto& entry = m_table(hash(key));

        if (entry.state == Record::State::ACTIVE)
        {
            entry.state = Record::State::DELETED;
            m_count--;
        }
    }

    template <Hashable K, typename V>
    V& RandomHash<K, V>::operator[](const K& key)
    {
        auto& entry = m_table[hash(key)];

        if (entry.state == Record::State::ACTIVE)
            return entry.val;
        else
        {
            insert(key);
            return entry.val; // Might not work?
        }
    }

    template <Hashable K, typename V>
    const V& RandomHash<K, V>::operator[](const K& key) const
    {
        const auto& entry = m_table[hash(key)];

        if (entry.state == Record::State::ACTIVE)
            return entry.val;
        else
            throw std::out_of_range(std::format("Key: {} does not exist", key));
    }

    template <Hashable K, typename V>
    size_t RandomHash<K, V>::size(void) const
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
        return m_table[hash(key)].state == Record::State::ACTIVE;
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
        std::string s = "{\n";
        for (const auto& entry : m_table)
            s += std::format("  {}: {}\n", entry.key, entry.val);
        s += "}\n";
    }

    template <Hashable K, typename V>
    uint RandomHash<K, V>::hash(const K& key) const
    {
        for (uint i = 0; i < bucket_count(); i++)
        {
            uint index = (std::hash(key) + i + i*i) % bucket_count();

            if (m_table[index].state == Record::State::EMPTY)
                return index;
            if (m_table[index].key == key)
                return index;
        }

        // No bucket found
        rehash();
        return hash(key);
    }

    template <Hashable K, typename V>
    void RandomHash<K, V>::rehash(void)
    {
        uint newBucketCount = Util::next_prime(2 * bucket_count());

        RH_LOG(std::format("Rehashing: {} -> {}", bucket_count(), newBucketCount));

        auto oldTable = std::move(m_table);
        m_table = std::vector<Record<K, V>>(newBucketCount);
        m_count = 0;
        m_fullBuckets = 0;

        for (const auto& entry : oldTable)
            if (entry.state == Record::State::ACTIVE)
                insert(entry.key, entry.val);
    }
}
