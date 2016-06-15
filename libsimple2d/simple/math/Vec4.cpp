#include "math/Vec4.h"

NS_BEGIN

Vec4::Vec4()
: x(0.0f), y(0.0f), z(0.0f), w(0.0f)
{
}

Vec4::Vec4(float xx, float yy, float zz, float ww)
: x(xx), y(yy), z(zz), w(ww)
{
}

Vec4::Vec4(const Vec4& copy)
{
    set(copy);
}

Vec4::~Vec4()
{
}

bool Vec4::isZero() const
{
    return x == 0.0f && y == 0.0f && z == 0.0f && w == 0.0f;
}

bool Vec4::isOne() const
{
    return x == 1.0f && y == 1.0f && z == 1.0f && w == 1.0f;
}

//

void Vec4::set(float xx, float yy, float zz, float ww)
{
    this->x = xx;
    this->y = yy;
    this->z = zz;
    this->w = ww;
}

void Vec4::set(const Vec4& v)
{
    this->x = v.x;
    this->y = v.y;
    this->z = v.z;
    this->w = v.w;
}

const Vec4 Vec4::ZERO = Vec4(0.0f, 0.0f, 0.0f, 0.0f);
const Vec4 Vec4::ONE = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
const Vec4 Vec4::UNIT_X = Vec4(1.0f, 0.0f, 0.0f, 0.0f);
const Vec4 Vec4::UNIT_Y = Vec4(0.0f, 1.0f, 0.0f, 0.0f);
const Vec4 Vec4::UNIT_Z = Vec4(0.0f, 0.0f, 1.0f, 0.0f);
const Vec4 Vec4::UNIT_W = Vec4(0.0f, 0.0f, 0.0f, 1.0f);

NS_END