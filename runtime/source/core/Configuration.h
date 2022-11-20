#pragma once
#include <cstdint>

namespace rt
{

    struct viewport_state
    {
        uint32_t width;
        uint32_t height;

        bool valid;
    };

    struct window_state
    {
        const char* title;
        uint32_t width;
        uint32_t height;
        bool decorated;
        bool vsync;
    };

    struct state
    {
        window_state window;
    };

}