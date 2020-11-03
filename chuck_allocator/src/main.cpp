#include "ChunkAllocator.h"
#include <vector>

int main() {
    struct A {
        int a;
        double d;
        A(int a, double d) : a(a), d(d) {}
    };
    ChunkAllocator<A> alloc;
    auto p = alloc.allocate(1);
    alloc.construct(p, 0, 0.1);
    alloc.destroy(p);

    auto alloc2 = alloc;
    ChunkAllocator<A> alloc3;
    alloc3 = alloc2;

    std::vector<A, ChunkAllocator<A>> vec(1500, A(0, 0));
    vec[1400].a = 9;

    return 0;
}