#pragma once

#include "Math.h"

namespace rt
{

    namespace math
    {

        class vector2
        {
        public:
            vector2() : x(0.0f), y(0.0f)
            {
            }

            vector2(float value) : x(value), y(value)
            {
            }

            vector2(float x, float y) : x(x), y(y)
            {
            }

            vector2(const vector2& vec) : x(vec.x), y(vec.y)
            {
            }

            template <typename T> vector2(T x, T y) : x(static_cast<float>(x)), y(static_cast<float>(y))
            {
            }

            ~vector2() = default;

            inline void operator-=(const vector2& vec)
            {
                this->x -= vec.x;
                this->y -= vec.y;
            }

            inline vector2 operator-(const vector2& vec) const
            {
                return vector2(this->x - vec.x, this->y - vec.y);
            }

            inline void operator+=(const vector2& vec)
            {
                this->x += vec.x;
                this->y += vec.y;
            }

            inline vector2 operator+(const vector2& vec) const
            {
                return vector2(this->x + vec.x, this->y + vec.y);
            }

            inline void operator*=(const vector2& vec)
            {
                this->x *= vec.x;
                this->y *= vec.y;
            }

            inline vector2 operator*(const vector2& vec) const
            {
                return vector2(this->x * vec.x, this->y * vec.y);
            }

            inline void operator-=(const float factor)
            {
                this->x -= factor;
                this->y -= factor;
            }

            inline vector2 operator-(float factor) const
            {
                return vector2(this->x - factor, this->y - factor);
            }

            inline void operator+=(const float factor)
            {
                this->x += factor;
                this->y += factor;
            }

            inline vector2 operator+(float factor) const
            {
                return vector2(this->x + factor, this->y + factor);
            }

            inline void operator*=(const float factor)
            {
                this->x *= factor;
                this->y *= factor;
            }

            inline vector2 operator*(float factor) const
            {
                return vector2(this->x * factor, this->y * factor);
            }

            inline float length() const
            {
                return math::sqrt((this->x * this->x) + (this->y * this->y));
            }

            inline float length_squared() const
            {
                return (this->x * this->x) + (this->y * this->y);
            }

            inline void normalize()
            {
                float lengthSquared = length_squared();
                if (lengthSquared != 1.0f)
                {
                    float inverseSqrt = 1 / math::sqrt(lengthSquared);
                    this->x *= inverseSqrt;
                    this->y *= inverseSqrt;
                }
            }

            inline vector2 normalize() const
            {
                float lengthSquared = length_squared();
                if (lengthSquared != 1.0f)
                {
                    float inverseSqrt = 1 / math::sqrt(lengthSquared);
                    return vector2(this->x * inverseSqrt, this->y * inverseSqrt);
                }
                return vector2(*this);
            }

            float x;
            float y;
        };

    }

}