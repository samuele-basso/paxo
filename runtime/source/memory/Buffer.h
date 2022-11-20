#pragma once

#include <memory>

#include "memory/Memory.h"

namespace rt
{

    namespace memory
    {

        template <typename alloc = allocator::malloc_free> class buffer
        {
        public:
            buffer() : p_size(0), p_storage(nullptr), p_limit(nullptr)
            {
            }

            buffer(buffer&&) = delete;
            buffer(const buffer&) = delete;

            ~buffer()
            {
                if (p_storage)
                {
                    alloc::free(static_cast<char* const>(p_storage));
                    p_storage = nullptr;
                }
            }

            void init(size_t size)
            {
                p_size = size;
                p_storage = alloc::malloc(size);
                p_limit = ops::byte_offset(p_storage, p_size);
            }

            inline const void* storage() const
            {
                return p_storage;
            }

            inline const void* limit() const
            {
                return p_limit;
            }

            inline size_t size() const
            {
                return p_size;
            }

        protected:
            size_t p_size;
            void* p_storage;
            void* p_limit;
        };

    }

}