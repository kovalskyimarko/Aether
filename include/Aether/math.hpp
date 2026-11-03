#pragma once

#include <cmath>

namespace Aether
{
    constexpr float PI = 3.14159265358979323846f;

    inline float ToRadians(float degrees) { return degrees * (PI / 180.0f); }
    inline float ToDegrees(float radians) { return radians * (180.0f / PI); }

    struct Vec2
    {
        float x = 0.0f, y = 0.0f;
    };

    struct Vec3
    {
        float x = 0.0f, y = 0.0f, z = 0.0f;

        Vec3 operator+(const Vec3& o) const { return {x + o.x, y + o.y, z + o.z}; }
        Vec3 operator-(const Vec3& o) const { return {x - o.x, y - o.y, z - o.z}; }
        Vec3 operator*(float s) const { return {x * s, y * s, z * s}; }
        Vec3 operator/(float s) const { return {x / s, y / s, z / s}; }

        Vec3& operator+=(const Vec3& o) { x += o.x; y += o.y; z += o.z; return *this; }
        Vec3& operator-=(const Vec3& o) { x -= o.x; y -= o.y; z -= o.z; return *this; }
        Vec3& operator*=(float s) { x *= s; y *= s; z *= s; return *this; }

        float LengthSq() const { return x * x + y * y + z * z; }
        float Length() const { return std::sqrt(LengthSq()); }

        Vec3 Normalize() const
        {
            float len = Length();
            if (len <= 1e-6f) return {0.0f, 0.0f, 0.0f};
            float invLen = 1.0f / len;
            return {x * invLen, y * invLen, z * invLen};
        }

        Vec3 Cross(const Vec3& o) const
        {
            return {
                y * o.z - z * o.y,
                z * o.x - x * o.z,
                x * o.y - y * o.x
            };
        }

        float Dot(const Vec3& o) const
        {
            return x * o.x + y * o.y + z * o.z;
        }
    };

    struct Mat4
    {
        float m[4][4] = {
            {0.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f, 0.0f}
        };

        static Mat4 Identity()
        {
            Mat4 res;
            res.m[0][0] = 1.0f;
            res.m[1][1] = 1.0f;
            res.m[2][2] = 1.0f;
            res.m[3][3] = 1.0f;
            return res;
        }

        /* Constructs standard right-handed LookAt view matrix */
        static Mat4 LookAt(const Vec3& eye, const Vec3& center, const Vec3& up)
        {
            Vec3 f = (center - eye).Normalize();
            Vec3 s = f.Cross(up).Normalize();
            Vec3 u = s.Cross(f);

            Mat4 res = Identity();
            res.m[0][0] = s.x;
            res.m[0][1] = u.x;
            res.m[0][2] = -f.x;

            res.m[1][0] = s.y;
            res.m[1][1] = u.y;
            res.m[1][2] = -f.y;

            res.m[2][0] = s.z;
            res.m[2][1] = u.z;
            res.m[2][2] = -f.z;

            res.m[3][0] = -s.Dot(eye);
            res.m[3][1] = -u.Dot(eye);
            res.m[3][2] =  f.Dot(eye);

            return res;
        }

        static Mat4 Perspective(float fovRad, float aspect, float nearPlane, float farPlane)
        {
            Mat4 res;
            float tanHalfFov = std::tan(fovRad * 0.5f);

            res.m[0][0] = 1.0f / (aspect * tanHalfFov);
            res.m[1][1] = 1.0f / tanHalfFov;
            res.m[2][2] = -(farPlane + nearPlane) / (farPlane - nearPlane);
            res.m[2][3] = -1.0f;
            res.m[3][2] = -(2.0f * farPlane * nearPlane) / (farPlane - nearPlane);
            return res;
        }

        Mat4 operator*(const Mat4& o) const
        {
            Mat4 res;
            for (int c = 0; c < 4; ++c)
            {
                for (int r = 0; r < 4; ++r)
                {
                    res.m[c][r] = m[0][r] * o.m[c][0] +
                                  m[1][r] * o.m[c][1] +
                                  m[2][r] * o.m[c][2] +
                                  m[3][r] * o.m[c][3];
                }
            }
            return res;
        }

        static Mat4 Translate(const Vec3& v)
        {
            Mat4 res = Identity();
            res.m[3][0] = v.x;
            res.m[3][1] = v.y;
            res.m[3][2] = v.z;
            return res;
        }

        static Mat4 Scale(const Vec3& v)
        {
            Mat4 res = Identity();
            res.m[0][0] = v.x;
            res.m[1][1] = v.y;
            res.m[2][2] = v.z;
            return res;
        }

        static Mat4 RotateX(float angleRad)
        {
            Mat4 res = Identity();
            float c = std::cos(angleRad), s = std::sin(angleRad);
            res.m[1][1] = c;  res.m[2][1] = -s;
            res.m[1][2] = s;  res.m[2][2] = c;
            return res;
        }

        static Mat4 RotateY(float angleRad)
        {
            Mat4 res = Identity();
            float c = std::cos(angleRad), s = std::sin(angleRad);
            res.m[0][0] = c;  res.m[2][0] = s;
            res.m[0][2] = -s; res.m[2][2] = c;
            return res;
        }

        static Mat4 RotateZ(float angleRad)
        {
            Mat4 res = Identity();
            float c = std::cos(angleRad), s = std::sin(angleRad);
            res.m[0][0] = c;  res.m[1][0] = -s;
            res.m[0][1] = s;  res.m[1][1] = c;
            return res;
        }
    };
}