#ifndef RECORD_HPP
#define RECORD_HPP

namespace RH
{
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
        v val;
        RecordState state;
    };
}

#endif // RECORD_HPP