#include "math/Vec3.h"

NS_BEGIN

Vec3::Vec3()
: x(0.0f), y(0.0f), z(0.0f)
{
}

Vec3::Vec3(float xx, float yy, float zz)
: x(xx), y(yy), z(zz)
{
}

Vec3::Vec3(const Vec3& copy)
{
    set(copy);
}

Vec3::~Vec3()
{
}

const Vec3 Vec3::ZERO(0.0f, 0.0f, 0.0f);
const Vec3 Vec3::ONE(1.0f, 1.0f, 1.0f);
const Vec3 Vec3::UNIT_X(1.0f, 0.0f, 0.0f);
const Vec3 Vec3::UNIT_Y(0.0f, 1.0f, 0.0f);
const Vec3 Vec3::UNIT_Z(0.0f, 0.0f, 1.0f);

NS_END