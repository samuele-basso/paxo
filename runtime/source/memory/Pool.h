#pragma once

#include <memory>
#include <list>

#include "Memory.h"
#include "Handle.h"
#include "core/Configuration.h"

#define RT_MEMORY_POOL_MULTIPLIER 1024

namespace rt
{

    namespace memory
    {

        /**
         * @brief manage storage creation and bounds.
         */
        template <typename T, size_t multiplier = RT_MEMORY_POOL_MULTIPLIER, size_t alignment = alignof(T)>
        struct pool_block
        {
            pool_block() : top(0), max(multiplier * ops::bit_size<uint64_t>()) { }

            /**
             * @var next free element.
             */
            size_t top;

            /**
             * @var storage size limit.
             */
            size_t max;

            ops::aligned_object<T, alignment> storage[multiplier * ops::bit_size<uint64_t>()];
        };

        /**
         * @brief handle creation and destruction.
         */
        template <typename T, size_t alignment = alignof(T)> struct pool_handle_system
        {
            pool_handle_system()
                : links(nullptr), reverse_links(nullptr), size(0), avaiable_links(nullptr), avaiable_links_top(0) { }

            /**
             * @var maps memory locations to active handles. Used to defragment memory and maintain handle validity.
             */
            uint32_t* reverse_links;

            /**
             * @var pool side handles.
             */
            link<ops::aligned_object<T, alignment>>* links;

            /**
             * @var keeps track of free links. Stack structure.
             */
            uint32_t* avaiable_links;
            uint32_t avaiable_links_top;

            uint32_t size;
        };

        template <typename T, typename IT, size_t multiplier = RT_MEMORY_POOL_MULTIPLIER, size_t alignment = alignof(T)>
        struct pool_iterator
        {
            pool_iterator(IT block, IT end)
                : block(block), end(end), current_element(0) { }

            size_t current_element;
            IT block;
            IT end;

            ops::aligned_object<T, alignment>* next()
            {
                if (current_element < block->top)
                {
                    return &block->storage[current_element++];
                }
                else
                {
                    block++;
                    current_element = 0;
                    if (block != end)
                    {
                        return next();
                    }
                    else
                    {
                        return nullptr;
                    }
                }
            }
        };

        template <typename T, size_t multiplier = RT_MEMORY_POOL_MULTIPLIER, size_t alignment = alignof(T)> class pool
        {
        public:
            pool()
            {
                expand();
                RT_TRACE("Pool created:\n\tMultiplier: {}\n\tAlignment: {}", multiplier, alignment);
            }

            ~pool()
            {
                if (p_handle_system.links) delete[] p_handle_system.links;
                if (p_handle_system.reverse_links) delete[] p_handle_system.reverse_links;
                if (p_handle_system.avaiable_links) delete[] p_handle_system.avaiable_links;
            }

            /**
             * @brief retrieve pointer from handle.
             * @returns nullptr if the handle is not valid.
             */
            ops::aligned_object<T, alignment>* dereference(handle<ops::aligned_object<T, alignment>>& handle)
            {
                auto* link = &p_handle_system.links[handle.value];
                /**
                 * check handle validity.
                 */
                if (link)
                {
                    if (link->pattern == handle.pattern)
                    {
                        return link->instance;
                    }
                }
                return nullptr;
            }

            decltype(auto) iterator()
            {
                return pool_iterator<T, std::list<pool_block<T, multiplier, alignment>>::iterator, multiplier, alignment>(
                    p_blocks.begin(),
                    p_blocks.end()
                );
            }

            /**
             * @brief create a new block. Expand all data structures to provide the necessary handles.
             */
            inline void expand()
            {
                RT_TRACE("Expanding pool");

                p_blocks.emplace_back();

                size_t new_size = p_handle_system.size + multiplier * ops::bit_size<uint64_t>();

                link<ops::aligned_object<T, alignment>>* swap_links = new link<ops::aligned_object<T, alignment>>[new_size];
                uint32_t* swap_reverse_links = new uint32_t[new_size];
                uint32_t* swap_avaiable_links = new uint32_t[new_size];

                if (p_handle_system.links)
                {
                    memcpy(swap_links, p_handle_system.links, p_handle_system.size * sizeof(link<ops::aligned_object<T, alignment>>));
                    delete[] p_handle_system.links;
                }

                if (p_handle_system.reverse_links)
                {
                    memcpy(swap_reverse_links, p_handle_system.reverse_links, p_handle_system.size * sizeof(uint32_t));
                    delete[] p_handle_system.reverse_links;
                }

                if (p_handle_system.avaiable_links)
                {
                    memcpy(swap_avaiable_links, p_handle_system.avaiable_links, p_handle_system.size * sizeof(uint32_t));
                    delete[] p_handle_system.avaiable_links;
                }

                p_handle_system.links = swap_links;
                p_handle_system.reverse_links = swap_reverse_links;
                p_handle_system.avaiable_links = swap_avaiable_links;
                for (size_t i = p_handle_system.size; i < new_size; i++)
                {
                    p_handle_system.avaiable_links[i] = i;
                }

                p_handle_system.size = new_size;
            }

            handle<ops::aligned_object<T, alignment>> malloc()
            {
                auto block_it = p_blocks.begin();
                size_t block = 0;

                while (block_it != p_blocks.end())
                {
                    if (block_it->top < block_it->max)
                    {
                        /**
                         * get next free memory location.
                         */
                        auto* allocated_location = &block_it->storage[block_it->top];

                        /**
                         * get next free link and initialize it.
                         */
                        auto link_index = p_handle_system.avaiable_links[p_handle_system.avaiable_links_top++];
                        link<ops::aligned_object<T, alignment>>* link = &p_handle_system.links[link_index];
                        link->pattern++;
                        link->instance = allocated_location;

                        /**
                         * map memory location to the link refering to it.
                         */
                        p_handle_system.reverse_links[block * multiplier * ops::bit_size<uint64_t>() + block_it->top++] = link_index;

                        return { link->pattern, link_index };
                    }
                    block_it++;
                    block++;
                }

                expand();
                return malloc();
            }

            /**
             * @param handle is the element to free. Will be overridden with the last element in the current block. The handles are updated accordingly.
             */
            bool free(handle<ops::aligned_object<T, alignment>>& handle)
            {
                auto* link = &p_handle_system.links[handle.value];
                if (link->pattern != handle.pattern) return false;
                auto* raw_pointer = link->instance;

                auto block_it = p_blocks.begin();
                size_t block = 0;

                while (block_it != p_blocks.end())
                {
                    if (raw_pointer < block_it->storage + block_it->max && raw_pointer >= block_it->storage)
                    {
                        auto* raw_last_pointer = &block_it->storage[block_it->top - 1];
                        block_it->top--;

                        *raw_pointer = *raw_last_pointer;

                        /**
                         * get last block link. This block must be updated with the new memory address.
                         */
                        uint32_t last_link_index = p_handle_system.reverse_links[block_it->top + block * multiplier * ops::bit_size<uint64_t>()];
                        auto* last_link = &p_handle_system.links[last_link_index];

                        last_link->instance = raw_pointer;

                        /**
                         * update reverse link. The position inside reverse_link is dictated by the memory location the link points to.
                         */
                        size_t reverse_index = raw_pointer - block_it->storage;
                        p_handle_system.reverse_links[block * multiplier * ops::bit_size<uint64_t>() + reverse_index] = last_link_index;

                        /**
                         * invalidate handle pattern.
                         */
                        link->pattern++;

                        p_handle_system.avaiable_links_top--;
                        p_handle_system.avaiable_links[p_handle_system.avaiable_links_top] = handle.value;
                    }
                    block_it++;
                    block++;
                }

                return false;
            }

        private:
            std::list<pool_block<T, multiplier, alignment>> p_blocks;
            pool_handle_system<T, alignment> p_handle_system;
        };

    }

}