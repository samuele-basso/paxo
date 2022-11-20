#pragma once
#include <stdint.h>
#include <memory>

namespace rt
{

    namespace memory
    {

        namespace ops
        {

            template <typename T> inline constexpr size_t bit_size()
            {
                return sizeof(T) * CHAR_BIT;
            }

            template <typename T> inline constexpr size_t byte_size()
            {
                return sizeof(T);
            }

            template <typename T> inline T* byte_offset(T* pointer, size_t offset)
            {
                uint8_t* ris = reinterpret_cast<uint8_t*>(pointer) + offset;
                return reinterpret_cast<T*>(ris);
            }

            inline bool is_aligned(void* pointer, size_t alignment)
            {
                size_t alignmentMask = alignment - 1;
                return !(reinterpret_cast<uintptr_t>(pointer) & alignmentMask);
            }

            inline uintptr_t align(uintptr_t pointer, size_t alignment)
            {
                uintptr_t diff = pointer & (alignment - 1);
                if (!diff)
                {
                    return pointer;
                }
                else
                {
                    return pointer + alignment - diff;
                }
            }

            inline size_t round(size_t size, size_t alignment)
            {
                size_t diff = size & (alignment - 1);
                if (!diff)
                {
                    return size;
                }
                else
                {
                    return size + alignment - diff;
                }
            }

            template <typename T, size_t alignment = alignof(T)> struct aligned_object
            {
                alignas(alignment) T instance;
            };

        }

        namespace allocator
        {

            struct new_delete
            {
                static char* malloc(const size_t bytes)
                {
                    return new (std::nothrow) char[bytes];
                }

                static void free(char* const block)
                {
                    delete[] block;
                }
            };

            struct malloc_free
            {
                static char* malloc(const size_t bytes)
                {
                    return static_cast<char*>(std::malloc(bytes));
                }

                static void free(char* const block)
                {
                    std::free(block);
                }
            };

        }

    }

}
