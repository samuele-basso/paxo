#pragma once

namespace rt
{

    namespace math
    {

        class vector4
        {
        public:
            vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f)
            {
            }

            vector4(float value) : x(value), y(value), z(value), w(value)
            {
            }

            vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)
            {
            }

            vector4(const vector4& vec) : x(vec.x), y(vec.y), z(vec.z), w(vec.w)
            {
            }

            template <typename T>
            vector4(T x, T y, T z, T w)
                : x(static_cast<float>(x)), y(static_cast<float>(y)), z(static_cast<float>(z)), w(static_cast<float>(w))
            {
            }

            ~vector4() = default;

            inline void operator-=(const vector4& vec)
            {
                this->x -= vec.x;
                this->y -= vec.y;
                this->z -= vec.z;
                this->w -= vec.w;
            }

            inline vector4 operator-(const vector4& vec) const
            {
                return vector4(this->x - vec.x, this->y - vec.y, this->z - vec.z, this->w - vec.w);
            }

            inline void operator+=(const vector4& vec)
            {
                this->x += vec.x;
                this->y += vec.y;
                this->z += vec.z;
                this->w += vec.w;
            }

            inline vector4 operator+(const vector4& vec) const
            {
                return vector4(this->x + vec.x, this->y + vec.y, this->z + vec.z, this->w + vec.w);
            }

            inline void operator*=(const vector4& vec)
            {
                this->x *= vec.x;
                this->y *= vec.y;
                this->z *= vec.z;
                this->w *= vec.w;
            }

            inline vector4 operator*(const vector4& vec) const
            {
                return vector4(this->x * vec.x, this->y * vec.y, this->z * vec.z, this->w * vec.w);
            }

            inline void operator-=(const float factor)
            {
                this->x -= factor;
                this->y -= factor;
                this->z -= factor;
                this->w -= factor;
            }

            inline vector4 operator-(float factor) const
            {
                return vector4(this->x - factor, this->y - factor, this->z - factor, this->w - factor);
            }

            inline void operator+=(const float factor)
            {
                this->x += factor;
                this->y += factor;
                this->z += factor;
                this->w += factor;
            }

            inline vector4 operator+(float factor) const
            {
                return vector4(this->x + factor, this->y + factor, this->z + factor, this->w + factor);
            }

            inline void operator*=(const float factor)
            {
                this->x *= factor;
                this->y *= factor;
                this->z *= factor;
                this->w *= factor;
            }

            inline vector4 operator*(float factor) const
            {
                return vector4(this->x * factor, this->y * factor, this->z * factor, this->w * factor);
            }

            float x;
            float y;
            float z;
            float w;
        };

    }

}
