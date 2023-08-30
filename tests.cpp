#include <cstdio>
#include <cstddef>
#include <memory>

#include "rbitset.hpp"

using RB::RuntimeBitset;


std::unique_ptr<RuntimeBitset<>[]> bitsetGridGen(const size_t rows, const size_t cols) {
    auto grid = std::make_unique<RuntimeBitset<>[]>(rows);

    for (size_t i = 0; i < rows; i++) {
        grid[i] = RuntimeBitset<>(cols, false);
    }

    return grid;
}

int main() {
    const size_t rows = 8, cols = 30;

    auto grid = bitsetGridGen(rows, cols);

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            grid[i][j] = (i%2 == j%2);
            printf("%hhx ", (bool) grid[i][j]);
        }
        printf("\n");
    }

    printf("Each bitset: %lu bools in %lu bytes.\n", grid[0].size(), grid[0].bytes());

    return 0;
}

/*
HEAP SUMMARY:
==87558==     in use at exit: 0 bytes in 0 blocks
==87558==   total heap usage: 11 allocs, 11 frees, 75,016 bytes allocated
==87558==
==87558== All heap blocks were freed -- no leaks are possible
==87558==
==87558== For lists of detected and suppressed errors, rerun with: -s
==87558== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
*/