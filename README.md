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

# Includes

Here are the includes used by the library:
+ `<cstring>` → Used for `memset()` and `memcpy()`. You can change the precompiler define `RUNTIME_BITSET_USE_CSTRING_` to false if necessary, which will adjust the code to `for` loop alternatives.
+ `<cstddef>` → Used only for standard C types, specifically `size_t`.

# Contributing and License

All code is under MIT, enjoy. Feel free to help, report issues, fork or send PRs if you want!

**Note:** I created this because I needed a runtime bitset with a simple implementation, and reasonably small memory footprint, for my other project at `magnetrwn/margolus-cpp`, but it's very likely to be still inefficient in access time versus compile-time `std::bitset`.