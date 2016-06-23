#ifndef __BASE_MAT4_H__
#define __BASE_MAT4_H__

#include "math/Vec3.h"

class DLL Mat4
{
public:
    float m[16];

    Mat4();

    Mat4(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24,
           float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44);

    ~Mat4();

    static void createOrthographic(float left, float right, float bottom, float top, float zNearPlane, float zFarPlane, Mat4* dst);

    inline void transformPoint(const Vec3& point, Vec3* dst) const;
    
    void set(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24,
             float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44);

    static const Mat4 ZERO;

    static const Mat4 IDENTITY;
};

#include "math/Mat4.inl"

#endif