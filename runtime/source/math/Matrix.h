#pragma once

#include "Math.h"
#include "Quaternion.h"
#include "Vector3.h"
#include "Vector4.h"

namespace rt
{
    namespace math
    {

        class matrix
        {
        public:
            matrix()
            {
                c0_r0 = 1.0f;
                c0_r1 = 0.0f;
                c0_r2 = 0.0f;
                c0_r3 = 0.0f;
                c1_r0 = 0.0f;
                c1_r1 = 1.0f;
                c1_r2 = 0.0f;
                c1_r3 = 0.0f;
                c2_r0 = 0.0f;
                c2_r1 = 0.0f;
                c2_r2 = 1.0f;
                c2_r3 = 0.0f;
                c3_r0 = 0.0f;
                c3_r1 = 0.0f;
                c3_r2 = 0.0f;
                c3_r3 = 1.0f;
            }

            matrix(
                float c0_r0, float c0_r1, float c0_r2, float c0_r3,
                float c1_r0, float c1_r1, float c1_r2, float c1_r3,
                float c2_r0, float c2_r1, float c2_r2, float c2_r3,
                float c3_r0, float c3_r1, float c3_r2, float c3_r3
            )
            {
                this->c0_r0 = c0_r0;
                this->c0_r1 = c0_r1;
                this->c0_r2 = c0_r2;
                this->c0_r3 = c0_r3;
                this->c1_r0 = c1_r0;
                this->c1_r1 = c1_r1;
                this->c1_r2 = c1_r2;
                this->c1_r3 = c1_r3;
                this->c2_r0 = c2_r0;
                this->c2_r1 = c2_r1;
                this->c2_r2 = c2_r2;
                this->c2_r3 = c2_r3;
                this->c3_r0 = c3_r0;
                this->c3_r1 = c3_r1;
                this->c3_r2 = c3_r2;
                this->c3_r3 = c3_r3;
            }

            ~matrix() = default;

            static inline matrix rotate(const quaternion& quat)
            {
                float xx = quat.x * quat.x;
                float xy = quat.x * quat.y;
                float xz = quat.x * quat.z;
                float yy = quat.y * quat.y;
                float yz = quat.y * quat.z;
                float zz = quat.z * quat.z;
                float wx = quat.w * quat.x;
                float wy = quat.w * quat.y;
                float wz = quat.w * quat.z;

                matrix m;

                m.c0_r0 = 1.0f - (2.0f * (yy + zz));
                m.c0_r1 = 2.0f * (xy + wz);
                m.c0_r2 = 2.0f * (xz - wy);
                m.c1_r0 = 2.0f * (xy - wz);
                m.c1_r1 = 1.0f - (2.0f * (xx + zz));
                m.c1_r2 = 2.0f * (yz + wx);
                m.c2_r0 = 2.0f * (xz + wy);
                m.c2_r1 = 2.0f * (yz - wx);
                m.c2_r2 = 1.0f - (2.0f * (xx + yy));

                return m;
            }

            static inline matrix translate(const vector3& position)
            {
                return matrix(
                    1.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, 1.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 1.0f, 0.0f,
                    position.x, position.y, position.z, 1.0f
                );
            }

            static inline matrix scale(const vector3& scale)
            {
                return matrix(
                    scale.x, 0.0f, 0.0f, 0.0f,
                    0.0f, scale.y, 0.0f, 0.0f,
                    0.0f, 0.0f, scale.z, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f
                );
            }

            float c0_r0, c0_r1, c0_r2, c0_r3;
            float c1_r0, c1_r1, c1_r2, c1_r3;
            float c2_r0, c2_r1, c2_r2, c2_r3;
            float c3_r0, c3_r1, c3_r2, c3_r3;
        };

        inline matrix operator*(const matrix& lhs, const matrix& rhs)
        {
            return matrix(
                lhs.c0_r0 * rhs.c0_r0 + lhs.c1_r0 * rhs.c0_r1 + lhs.c2_r0 * rhs.c0_r2 + lhs.c3_r0 * rhs.c0_r3,
                lhs.c0_r1 * rhs.c0_r0 + lhs.c1_r1 * rhs.c0_r1 + lhs.c2_r1 * rhs.c0_r2 + lhs.c3_r1 * rhs.c0_r3,
                lhs.c0_r2 * rhs.c0_r0 + lhs.c1_r2 * rhs.c0_r1 + lhs.c2_r2 * rhs.c0_r2 + lhs.c3_r2 * rhs.c0_r3,
                lhs.c0_r3 * rhs.c0_r0 + lhs.c1_r3 * rhs.c0_r1 + lhs.c2_r3 * rhs.c0_r2 + lhs.c3_r3 * rhs.c0_r3,

                lhs.c0_r0 * rhs.c1_r0 + lhs.c1_r0 * rhs.c1_r1 + lhs.c2_r0 * rhs.c1_r2 + lhs.c3_r0 * rhs.c1_r3,
                lhs.c0_r1 * rhs.c1_r0 + lhs.c1_r1 * rhs.c1_r1 + lhs.c2_r1 * rhs.c1_r2 + lhs.c3_r1 * rhs.c1_r3,
                lhs.c0_r2 * rhs.c1_r0 + lhs.c1_r2 * rhs.c1_r1 + lhs.c2_r2 * rhs.c1_r2 + lhs.c3_r2 * rhs.c1_r3,
                lhs.c0_r3 * rhs.c1_r0 + lhs.c1_r3 * rhs.c1_r1 + lhs.c2_r3 * rhs.c1_r2 + lhs.c3_r3 * rhs.c1_r3,

                lhs.c0_r0 * rhs.c2_r0 + lhs.c1_r0 * rhs.c2_r1 + lhs.c2_r0 * rhs.c2_r2 + lhs.c3_r0 * rhs.c2_r3,
                lhs.c0_r1 * rhs.c2_r0 + lhs.c1_r1 * rhs.c2_r1 + lhs.c2_r1 * rhs.c2_r2 + lhs.c3_r1 * rhs.c2_r3,
                lhs.c0_r2 * rhs.c2_r0 + lhs.c1_r2 * rhs.c2_r1 + lhs.c2_r2 * rhs.c2_r2 + lhs.c3_r2 * rhs.c2_r3,
                lhs.c0_r3 * rhs.c2_r0 + lhs.c1_r3 * rhs.c2_r1 + lhs.c2_r3 * rhs.c2_r2 + lhs.c3_r3 * rhs.c2_r3,

                lhs.c0_r0 * rhs.c3_r0 + lhs.c1_r0 * rhs.c3_r1 + lhs.c2_r0 * rhs.c3_r2 + lhs.c3_r0 * rhs.c3_r3,
                lhs.c0_r1 * rhs.c3_r0 + lhs.c1_r1 * rhs.c3_r1 + lhs.c2_r1 * rhs.c3_r2 + lhs.c3_r1 * rhs.c3_r3,
                lhs.c0_r2 * rhs.c3_r0 + lhs.c1_r2 * rhs.c3_r1 + lhs.c2_r2 * rhs.c3_r2 + lhs.c3_r2 * rhs.c3_r3,
                lhs.c0_r3 * rhs.c3_r0 + lhs.c1_r3 * rhs.c3_r1 + lhs.c2_r3 * rhs.c3_r2 + lhs.c3_r3 * rhs.c3_r3
            );
        }

        inline vector4 operator*(const matrix& lhs, const vector4& rhs)
        {
            return vector4(
                lhs.c0_r0 * rhs.x + lhs.c1_r0 * rhs.y + lhs.c2_r0 * rhs.z + lhs.c3_r0 * rhs.w,
                lhs.c0_r1 * rhs.x + lhs.c1_r1 * rhs.y + lhs.c2_r1 * rhs.z + lhs.c3_r1 * rhs.w,
                lhs.c0_r2 * rhs.x + lhs.c1_r2 * rhs.y + lhs.c2_r2 * rhs.z + lhs.c3_r2 * rhs.w,
                lhs.c0_r3 * rhs.x + lhs.c1_r3 * rhs.y + lhs.c2_r3 * rhs.z + lhs.c3_r3 * rhs.w
            );
        }

        inline matrix perspective(float aspect, float fov, float far_plane, float near_plane)
        {
            float tanHFOV = math::tan(fov / 2.0f);

            return matrix(
                1.0f / (aspect * tanHFOV), 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f / tanHFOV, 0.0f, 0.0f,
                0.0f, 0.0f, -(far_plane + near_plane) / (far_plane - near_plane), -1.0f,
                0.0f, 0.0f, -(2.0f * far_plane * near_plane) / (far_plane - near_plane), 0.0f
            );
        }

        inline matrix orthographic(float left, float right, float top, float bottom, float far_plane, float near_plane)
        {
            return matrix(
                2.0f / (right - left), 0.0f, 0.0f, 0.0f,
                0.0f, 2.0f / (top - bottom), 0.0f, 0.0f,
                0.0f, 0.0f, -2.0f / (far_plane - near_plane), 0.0f,
                -(right + left) / (right - left), -(top + bottom) / (top - bottom), -(far_plane + near_plane) / (far_plane - near_plane), 1.0f
            );
        }

        inline matrix look_at(vector3& eye, vector3& center, vector3& up)
        {
            vector3 f = center - eye;
            f.normalize();
            vector3 s = cross(f, up);
            s.normalize();
            vector3 u = cross(s, f);
            
            return matrix(
                s.x,  u.x,  -f.x, 0.0f,
                s.y,  u.y,  -f.y, 0.0f,
                s.z,  u.z,  -f.z, 0.0f,
                -dot(s, eye), -dot(u, eye), dot(f, eye), 1.0f
            );
        }

    }
}
