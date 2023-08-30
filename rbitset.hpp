#ifndef RUNTIME_BITSET_HEADER_
#define RUNTIME_BITSET_HEADER_

#define RUNTIME_BITSET_USE_CSTRING_ true

/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2023 magnetrwn
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <cstddef>

#if RUNTIME_BITSET_USE_CSTRING_
#include <cstring>
#endif

namespace RB {

    using defaultBlockType = size_t;

    template <typename> class BitReference;

    template <typename BlockType = defaultBlockType>
    class RuntimeBitset {
    protected:
        friend class BitReference<BlockType>;
        BlockType size_; // Number of BlockType elements to fit bits in data
        BlockType *data;

    public:
        inline RuntimeBitset()
            : size_(0),
            data(nullptr) {}

        inline RuntimeBitset(const size_t bits, const bool fill_with = false)
            : size_(bits / (sizeof(BlockType) * 8) + (size_t) !(bits % (sizeof(BlockType) * 8) == 0)),
            data(nullptr) {
            data = new BlockType[size_];

            #if RUNTIME_BITSET_USE_CSTRING_
            memset(data, 255*((size_t) fill_with) , size_ * sizeof(BlockType));
            #else
            for (BlockType i = 0; i < size_; ++i) {
                data[i] = (fill_with) ? ~0 : 0;
            }
            #endif
        }

        inline virtual ~RuntimeBitset() {
            if (data)
                delete[] data;
        }

        inline RuntimeBitset& operator=(const RuntimeBitset& lv) {
            if (this != &lv) {
                if (data)
                    delete[] data;
                size_ = lv.size_;
                data = new BlockType[size_];
                #if RUNTIME_BITSET_USE_CSTRING_
                memcpy(data, lv.data, size_ * sizeof(BlockType));
                #else
                for (BlockType i = 0; i < size_; ++i) {
                    data[i] = lv.data[i];
                }
                #endif
            }
            return *this;
        }

        inline RuntimeBitset& operator=(RuntimeBitset&& rv) noexcept {
            if (this != &rv) {
                delete[] data;
                size_ = rv.size_;
                data = rv.data;
                rv.data = nullptr;
            }
            return *this;
        }

        inline BitReference<BlockType> operator[](const size_t idx) {
            return BitReference<BlockType>(*this, idx);
        }

        inline size_t size() const {
            return size_;
        }

        inline size_t bits() const {
            return size_ * sizeof(BlockType) * 8;
        }
    };

    template <typename BlockType = defaultBlockType>
    class BitReference {
    protected:
        friend class RuntimeBitset<BlockType>;
        const RuntimeBitset<BlockType>& bitset_;
        const BlockType block_;
        const BlockType mask_;

        BitReference(const RuntimeBitset<BlockType>& bs, BlockType idx)
            : bitset_(bs), block_(idx / bs.bits()), mask_(static_cast<BlockType>(1) << (idx % bs.bits())) {}

    public:
        BitReference& operator=(bool value) {
            if (value)
                bitset_.data[block_] |= mask_;
            else
                bitset_.data[block_] &= ~mask_;
            return *this;
        }

        operator bool() const {
            return (bitset_.data[block_] & mask_) != 0;
        }
    };

}
#endif