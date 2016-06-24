#include "math/Mat4.h"

void Mat4::transformPoint(const Vec3& point, Vec3* dst) const
{


}

inline const Mat4 Mat4::operator*(const Mat4& mat) const
{
    return *this;
}

inline Mat4& Mat4::operator*=(const Mat4& mat)
{
    return *this;
}