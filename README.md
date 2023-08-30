# Runtime Bitset CPP
**This is a very compact header-only library for creating bitsets like `std::bitset` at runtime.**

```cpp
#include "rbitset.hpp"
using RB::RuntimeBitset;

// Simple syntax...
RuntimeBitset<> p(20);

// ...templated for selectable underlying types...
RuntimeBitset<uint32_t> q(20);

// ...with overloaded constructors...
RuntimeBitset<> r;
r = RuntimeBitset<>(20);

// ...and fill on creation...
RuntimeBitset<> s(20, true); // All true!

// ...as well as move and copy assignments.
r = s;            // Copy!
s = std::move(p); // Move!
```

# Installation

To use the library, you can:
```sh
cd lib
git clone https://github.com/magnetrwn/runtime-bitset-cpp
```
and include the library from its location, usually from inside `lib/` in your project root.

The library contains just two classes:
+ `RuntimeBitset` → Main wrapper for bitsets
+ `BitReference` → Friend class, returned by accessing a bit in the bitset

# Operations

When working with a RuntimeBitset, index accessing has a bit of a quirk:
```cpp
#include "rbitset.hpp"
using RB::RuntimeBitset;

bool demonstratingBitsets() {
    RuntimeBitset<> bs(20);

    bs[0] = true;
    // Calls operator[] on RuntimeBitset instance, which returns BitReference.
    // BitReference operator= is then called to set the value.

    auto firstIndexRef = bs[0];
    // This might be confused as getting the bool rvalue, but
    // it's actually returning a BitReference to the requested bit!

    return (bool) bs[0] == (bool) firstIndexRef;
    // To actually get the rvalue, casting to bool through BitReference's
    // operator bool() is required.
}

```

BitReference is able to act on a single bit by using a bit mask stored in the instance, together with a block index to locate the block where the bit is located.

# Includes

Here are the includes used by the library:
+ `<cstring>` → Used for `memset()` and `memcpy()`. You can change the precompiler define `RUNTIME_BITSET_USE_CSTRING_` to false if necessary, which will adjust the code to `for` loop alternatives.
+ `<cstddef>` → Used only for standard C types, specifically `size_t`.

# Speed

`benchmark.cpp` basically has simple benchmarks to test speed against other possible implementations of data structures using bitsets and arrays, and more. Cache linearity is a relevant factor here, so the benchmark tries to be a little randomic as well.

Here is a sample run from running `benchmark.cpp`:
```
$ ./benchmark
Running 24 rounds with 480 rows and 640 cols...
------------------------------------------------------------
| A | RuntimeBitset<>(cols)[rows] completed in: 95237us.
| B | std::array<std::bitset<cols>, rows> completed in: 85988us.
| C | std::deque<std::deque<bool>> completed in: 143360us.
------------------------------------------------------------
A takes +10.8% time than B.
A takes -33.6% time than C.
```
Of course results on different machines will change, but there is consistency between results from runs, at around ±5%.

# Examples

This is a simple way to implement a memory efficient grid (although I guess it can be improved even more by making it all fit in a single big bitset and then offset indexes?):
```cpp
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

    return 0;
}
```
When run:
```
$ ./example
1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0
0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1
1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0
0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1
1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0
0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1
1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0
0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1
```

# Contributing and License

All code is under MIT, enjoy. Feel free to help, report issues, fork or send PRs if you want!

**Note:** I created this because I needed a runtime bitset with a simple implementation, and reasonably small memory footprint, for my other project at `magnetrwn/margolus-cpp`, but it's very likely to be still inefficient in access time versus compile-time `std::bitset`.