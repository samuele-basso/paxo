#pragma once
#include "memory/Memory.h"
#include "core/Log.h"

namespace rt
{
    namespace container
    {

        template <typename T, size_t initial_size, typename alloc = memory::allocator::malloc_free> class vector
        {
        public:
            vector() : p_storage(nullptr), p_swap(nullptr), p_size(initial_size), p_count(0)
            {
                RT_STATIC_ASSERT(initial_size > 0);

                p_storage = reinterpret_cast<T*>(alloc::malloc(sizeof(T) * initial_size));
                RT_ASSERT(p_storage);
                new (p_storage) T[p_size];
            }

            vector(const vector&) = delete;

            ~vector<T, initial_size, alloc>()
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
                    p_count = 0;
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

            void insert(T& value)
            {
                RT_ASSERT(p_count < p_size);
                p_storage[p_count++] = value;
            }

            void remove(size_t index)
            {
                RT_ASSERT(index < p_count);
                p_storage[index].~T();
                p_storage[index] = p_storage[p_count - 1];
                p_count--;
            }

            T& at(size_t index)
            {
                RT_ASSERT(index < p_count);
                return p_storage[index];
            }

            inline size_t size() const
            {
                return p_size;
            }

            inline size_t count() const
            {
                return p_count;
            }

        private:
            T* p_storage;
            T* p_swap;

            size_t p_size;
            size_t p_count;
        };

        template <typename T, size_t initial_size, typename alloc = memory::allocator::malloc_free> class sorted_vector
        {
        public:
            sorted_vector() : p_storage(nullptr), p_swap(nullptr), p_size(initial_size), p_count(0)
            {
                RT_STATIC_ASSERT(initial_size > 0);

                p_storage = reinterpret_cast<T*>(alloc::malloc(sizeof(T) * initial_size));
                RT_ASSERT(p_storage);
                new (p_storage) T[p_size];
            }

            sorted_vector(const sorted_vector&) = delete;

            ~sorted_vector<T, initial_size, alloc>()
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
                    p_count = 0;
                    p_size = 0;
                }
            }

            void expand(size_t size)
            {
                if (size > p_size)
                {
                    p_swap = reinterpret_cast<T*>(alloc::malloc(sizeof(T) * size));
                    new (p_swap + p_size) T[size - p_size];
                    memcpy(p_swap, p_storage, sizeof(T) * p_count);
                    p_size = size;
                    if (p_storage)
                    {
                        alloc::free(reinterpret_cast<char* const>(p_storage));
                    }
                    p_storage = p_swap;
                    p_swap = nullptr;
                }
            }

            void insert(T& value)
            {
                RT_ASSERT(p_count < p_size);

                size_t index = find(value);

                if (p_count > index)
                {
                    memmove(p_storage + index + 1, p_storage + index, (p_count - index) * sizeof(T));
                }

                p_storage[index] = value;

                p_count++;
            }

            size_t find(T& value)
            {
                size_t min = 0;
                size_t max = p_count;
                size_t mdl = 0;

                while (min < max)
                {
                    mdl = (min + max) / 2;
                    if (p_storage[mdl] < value)
                    {
                        min = mdl + 1;
                    }
                    else
                    {
                        max = mdl;
                    }
                }

                return min;
            }

            void remove(size_t index)
            {
                RT_ASSERT(index < p_count);
                p_storage[index].~T();
                memmove(p_storage + index, p_storage + index + 1, (p_count - index - 1) * sizeof(T));
                p_count--;
            }

            T& at(size_t index)
            {
                RT_ASSERT(index < p_count);
                return p_storage[index];
            }

            inline size_t size() const
            {
                return p_size;
            }

            inline size_t count() const
            {
                return p_count;
            }

        private:
            T* p_storage;
            T* p_swap;

            size_t p_size;
            size_t p_count;
        };

    }
}