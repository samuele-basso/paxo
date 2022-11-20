#pragma once

#include "Math.h"

#include "Vector4.h"

namespace rt
{

    namespace math
    {

        class vector3
        {
        public:
            vector3() : x(0.0f), y(0.0f), z(0.0f)
            {
            }

            vector3(float value) : x(value), y(value), z(value)
            {
            }

            vector3(float x, float y, float z) : x(x), y(y), z(z)
            {
            }

            vector3(const vector3& vec) : x(vec.x), y(vec.y), z(vec.z)
            {
            }

            vector3(const vector4& vec) : x(vec.x), y(vec.y), z(vec.z)
            {
            }

            template <typename T>
            vector3(T x, T y, T z) : x(static_cast<float>(x)), y(static_cast<float>(y)), z(static_cast<float>(z))
            {
            }

            ~vector3() = default;

            inline void operator-=(const vector3& vec)
            {
                this->x -= vec.x;
                this->y -= vec.y;
                this->z -= vec.z;
            }

            inline vector3 operator-(const vector3& vec) const
            {
                return vector3(this->x - vec.x, this->y - vec.y, this->z - vec.z);
            }

            inline void operator+=(const vector3& vec)
            {
                this->x += vec.x;
                this->y += vec.y;
                this->z += vec.z;
            }

            inline vector3 operator+(const vector3& vec) const
            {
                return vector3(this->x + vec.x, this->y + vec.y, this->z + vec.z);
            }

            inline void operator*=(const vector3& vec)
            {
                this->x *= vec.x;
                this->y *= vec.y;
                this->z *= vec.z;
            }

            inline vector3 operator*(const vector3& vec) const
            {
                return vector3(this->x * vec.x, this->y * vec.y, this->z * vec.z);
            }

            inline void operator-=(float factor)
            {
                this->x -= factor;
                this->y -= factor;
                this->z -= factor;
            }

            inline vector3 operator-(float factor) const
            {
                return vector3(this->x - factor, this->y - factor, this->z - factor);
            }

            inline void operator+=(float factor)
            {
                this->x += factor;
                this->y += factor;
                this->z += factor;
            }

            inline vector3 operator+(float factor) const
            {
                return vector3(this->x + factor, this->y + factor, this->z + factor);
            }

            inline void operator*=(float factor)
            {
                this->x *= factor;
                this->y *= factor;
                this->z *= factor;
            }

            inline vector3 operator*(float factor) const
            {
                return vector3(this->x * factor, this->y * factor, this->z * factor);
            }

            inline float dot(const vector3& vec) const
            {
                return (this->x * vec.x) + (this->y * vec.y) + (this->z * vec.z);
            }

            inline vector3 cross(const vector3& vec) const
            {
                return vector3((this->y * vec.z - this->z * vec.y),
                    -(this->x * vec.z - this->z * vec.x),
                    (this->x * vec.y - this->y * vec.x));
            }

            inline float length() const
            {
                return math::sqrt((this->x * this->x) + (this->y * this->y) + (this->z * this->z));
            }

            inline float length_squared() const
            {
                return (this->x * this->x) + (this->y * this->y) + (this->z * this->z);
            }

            inline void normalize()
            {
                float lengthSquared = length_squared();
                if (lengthSquared != 1.0f)
                {
                    float inverseSqrt = 1 / math::sqrt(lengthSquared);
                    this->x *= inverseSqrt;
                    this->y *= inverseSqrt;
                    this->z *= inverseSqrt;
                }
            }

            inline vector3 normalize() const
            {
                float lengthSquared = length_squared();
                if (lengthSquared != 1.0f)
                {
                    float inverseSqrt = 1 / math::sqrt(lengthSquared);
                    return vector3(this->x * inverseSqrt, this->y * inverseSqrt, this->z * inverseSqrt);
                }
                return vector3(*this);
            }

            float x;
            float y;
            float z;
        };

        inline float dot(const vector3& lhs, const vector3& rhs)
        {
            return (lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z);
        }

        inline vector3 cross(const vector3& lhs, const vector3& rhs)
        {
            return vector3((lhs.y * rhs.z - lhs.z * rhs.y), -(lhs.x * rhs.z - lhs.z * rhs.x), (lhs.x * rhs.y - lhs.y * rhs.x));
        }

    }

}
