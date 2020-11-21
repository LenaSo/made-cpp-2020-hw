#ifndef MEANWHILE_IN_THE_SEVENTH_GALAXY_FIRST_VARIANT_GAME_H
#define MEANWHILE_IN_THE_SEVENTH_GALAXY_FIRST_VARIANT_GAME_H

#include "xor_sum.h"
#include "binary.h"

template <size_t Idx, size_t Head, size_t... Tail>
struct HeapAt {
    static constexpr size_t value = HeapAt<Idx - 1, Tail...>::value;
};

template <size_t Head, size_t... Tail>
struct HeapAt <0, Head, Tail...> {
    static constexpr size_t value = Head;
};


template <size_t... Heaps>
struct Game {
private:
    using XorSumBits = Log2<XorSum<Heaps...>::value>;

    template <size_t heapIdx>
    using CheckHeapIdx = GetBit<XorSumBits::value - 1,
                                HeapAt<heapIdx, Heaps...>::value>;

    template <bool cond, size_t idx>
    struct TrueIdx {
        static constexpr size_t value = TrueIdx<CheckHeapIdx<idx + 1>::value, idx + 1>::value;
    };
    template <size_t idx>
    struct TrueIdx <true, idx> {
        static constexpr size_t value = idx;
    };
    using FindHeapIndex = TrueIdx<CheckHeapIdx<0>::value, 0>;
    struct FindRocksCount {
        static constexpr size_t value = HeapAt<FindHeapIndex::value, Heaps...>::value -
                (HeapAt<FindHeapIndex::value, Heaps...>::value ^ XorSum<Heaps...>::value);
    };

public:
    static constexpr bool first_player_wins = XorSum<Heaps...>::value;
    static constexpr ssize_t first_move_heap_index = first_player_wins ? FindHeapIndex::value : -1;
    static constexpr ssize_t first_move_rocks_count = first_player_wins ? FindRocksCount::value : -1;

};


#endif /// MEANWHILE_IN_THE_SEVENTH_GALAXY_FIRST_VARIANT_GAME_H.
