#pragma once

#include <cmath>

namespace rt
{
    namespace math
    {

        template <typename T> constexpr inline T sqrt(T value)
        {
            return std::sqrt(value);
        }

        template <typename T> constexpr inline T cos(T value)
        {
            return std::cos(value);
        }

        template <typename T> constexpr inline T sin(T value)
        {
            return std::sin(value);
        }

        template <typename T> constexpr inline T tan(T value)
        {
            return std::tan(value);
        }

        template <typename T> constexpr inline T radians(T value)
        {
            return value * static_cast<T>(0.01745329251994329576923690768489);
        }

    }
}
