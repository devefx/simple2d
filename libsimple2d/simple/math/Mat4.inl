#include "math/Mat4.h"

NS_BEGIN

void Mat4::transformPoint(const Vec3& point, Vec3* dst) const
{
    float x = point.x, y = point.y, z = point.z, w = 1.0f;
    dst->x = x * m[0] + y * m[4] + z * m[8] + w * m[12];
    dst->y = x * m[1] + y * m[5] + z * m[9] + w * m[13];
    dst->z = x * m[2] + y * m[6] + z * m[10] + w * m[14];
}

inline const Mat4 Mat4::operator*(const Mat4& mat) const
{
    return *this;
}

inline Mat4& Mat4::operator*=(const Mat4& mat)
{
    return *this;
}

NS_END