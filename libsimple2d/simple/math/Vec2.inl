#include "math/Vec2.h"

NS_BEGIN

inline Vec2::Vec2()
: x(0.0f)
, y(0.0f)
{
}

inline Vec2::Vec2(float xx, float yy)
: x(xx)
, y(yy)
{
}

inline Vec2::Vec2(const Vec2& copy)
{
    set(copy);
}

inline Vec2::~Vec2()
{
}

inline bool Vec2::isZero() const
{
    return x == 0.0f && y == 0.0f;
}

inline bool Vec2::isOne() const
{
    return x == 1.0f && y == 1.0f;
}

inline void Vec2::set(float xx, float yy)
{
    this->x = xx;
    this->y = yy;
}

inline void Vec2::set(const Vec2& v)
{
    this->x = v.x;
    this->y = v.y;
}

NS_END