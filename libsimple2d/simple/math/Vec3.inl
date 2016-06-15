#include "math/Vec3.h"

NS_BEGIN

inline bool Vec3::isZero() const
{
    return x == 0.0f && y == 0.0f && z == 0.0f;
}

inline bool Vec3::isOne() const
{
    return x == 1.0f && y == 1.0f && z == 1.0f;
}

inline void Vec3::set(float xx, float yy, float zz)
{
    this->x = xx;
    this->y = yy;
    this->z = zz;
}

inline void Vec3::set(const Vec3& v)
{
    this->x = v.x;
    this->y = v.y;
    this->z = v.z;
}

NS_END