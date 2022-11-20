#pragma once

#include "Math.h"

namespace rt
{

    namespace math
    {

        class quaternion
        {
        public:
            quaternion() : w(0.0f), x(0.0f), y(0.0f), z(0.0f)
            {
            }

            quaternion(float w, float x, float y, float z) : w(w), x(x), y(y), z(z)
            {
            }

            ~quaternion() = default;

            float w;
            float x;
            float y;
            float z;
        };

        inline quaternion operator*(const quaternion& lhs, const quaternion& rhs)
        {
            quaternion q;
            q.w = lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z;
            q.x = lhs.w * rhs.x + lhs.x * rhs.w - lhs.y * rhs.z + lhs.z * rhs.y;
            q.y = lhs.w * rhs.y + lhs.x * rhs.z + lhs.y * rhs.w - lhs.z * rhs.x;
            q.z = lhs.w * rhs.z - lhs.x * rhs.y + lhs.y * rhs.x + lhs.z * rhs.w;

            return q;
        }

        inline quaternion attitude_to_quaternion(float roll, float pitch, float yaw)
        {
            float r = roll * 0.5f;
            float p = pitch * 0.5f;
            float y = yaw * 0.5f;

            float cy = math::cos(y);
            float sy = math::sin(y);
            float cp = math::cos(p);
            float sp = math::sin(p);
            float cr = math::cos(r);
            float sr = math::sin(r);

            quaternion q;
            q.w = cr * cp * cy + sr * sp * sy;
            q.x = sr * cp * cy - cr * sp * sy;
            q.y = cr * sp * cy + sr * cp * sy;
            q.z = cr * cp * sy - sr * sp * cy;

            return q;
        }

    }

}