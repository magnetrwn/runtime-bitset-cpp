#include <cstdio>
#include "rbitset.hpp"

using RB::RuntimeBitset;

int main() {
    RuntimeBitset<> a, b;
    a = RuntimeBitset<>(20);
    for (size_t i = 0; i < a.bits(); i++) {
        a[i] = (i%3 != 0);
        printf("%hhx ", (bool) a[i]);
    }
    printf("\n");
    b = a;
    //b = std::move(a);
    for (size_t i = 0; i < a.bits(); i++)
        printf("%hhx ", (bool) b[i]);
    printf("\n");
    for (size_t i = 0; i < a.bits(); i++)
        printf("%hhx ", (bool) a[i]);
    printf("\n");

    RuntimeBitset<> bs(20);
    bs[0] = true;
    auto firstIndexRef = bs[0];
    return (bool) bs[0];

    /*for (size_t n = 0; n < 3500; n++) {
        RuntimeBitset<BlockType> a(n);

        for (size_t i = 0; i < n; i++) {
            a[i] = true;
        }

        printf("\n%lu: \t", n);

        for (size_t i = 0; i < n; i++) {
            if (i%(sizeof(BlockType)*8) == 0 && i != 0)
                printf(" ");

            if ((bool) a[i] != true)
                printf("\x1B[41m");
            else
                printf("\x1B[92m");
            printf("%d\x1B[0m", (bool) a[i]);
        }

        printf("\nb: \t");

        for (size_t i = 0; i < n; i++) {
            if (i%(sizeof(BlockType)*8) == 0 && i != 0)
                printf(" ");
            printf("%lu", a[i].block_%10);
        }

        printf("\nm: \t");

        for (size_t i = 0; i < n; i++) {
            if (i%(sizeof(BlockType)*8) == 0 && i != 0)
                printf(" ");

            if (a[i].mask_%10 == 1)
                printf("\x1B[04m");
            printf("%lu\x1B[0m", a[i].mask_%10);
        }
    }*/

    printf("\n\nOK\n");
    return 0;
}
