#pragma once
#include "memory/Memory.h"
#include "core/Log.h"

namespace rt
{
    namespace container
    {

        template <typename T, size_t initial_size, typename alloc = memory::allocator::malloc_free> class dynamic_array
        {
        public:
            dynamic_array()
                : p_storage(nullptr), p_swap(nullptr), p_size(initial_size)
            {
                RT_STATIC_ASSERT(initial_size > 0);

                p_storage = reinterpret_cast<T*>(alloc::malloc(sizeof(T) * initial_size));
                RT_ASSERT(p_storage);
                new (p_storage) T[p_size];
            }

            dynamic_array(const dynamic_array&) = delete;

            ~dynamic_array<T, initial_size, alloc>()
            {
                if (p_storage)
                {
                    for (size_t i = 0; i < p_size; i++)
                    {
                        p_storage[i].~T();
                    }

                    alloc::free(reinterpret_cast<char* const>(p_storage));

                    p_storage = nullptr;
                    p_swap = nullptr;
                    p_size = 0;
                }
            }

            void expand(size_t size)
            {
                if (size > p_size)
                {
                    p_swap = reinterpret_cast<T*>(alloc::malloc(sizeof(T) * size));
                    new (p_swap + p_size) T[size - p_size];
                    memcpy(p_swap, p_storage, sizeof(T) * p_size);
                    p_size = size;
                    if (p_storage)
                    {
                        alloc::free(reinterpret_cast<char* const>(p_storage));
                    }
                    p_storage = p_swap;
                    p_swap = nullptr;
                }
            }

            T& operator[] (size_t index)
            {
                RT_ASSERT(index < p_size);
                return p_storage[index];
            }

            inline size_t size() const
            {
                return p_size;
            }

        private:
            T* p_storage;
            T* p_swap;

            size_t p_size;
        };

    }
}