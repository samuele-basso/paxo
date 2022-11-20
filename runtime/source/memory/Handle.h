#pragma once

#include <stdint.h>

namespace rt
{

    namespace memory
    {

        using unique = uint32_t;

        template <typename T> struct handle
        {
            unique pattern;
            uint32_t value;
        };

        template <typename T> struct link
        {
            unique pattern;
            T* instance;
        };

    }

}