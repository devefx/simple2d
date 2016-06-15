#include "math/Vec2.h"

NS_BEGIN

bool Vec2::equals(const Vec2& target) const
{
    return false;
}

const Vec2 Vec2::ZERO(0.0f, 0.0f);
const Vec2 Vec2::ONE(1.0f, 1.0f);
const Vec2 Vec2::UNIT_X(1.0f, 0.0f);
const Vec2 Vec2::UNIT_Y(0.0f, 1.0f);
const Vec2 Vec2::ANCHOR_MIDDLE(0.5f, 0.5f);
const Vec2 Vec2::ANCHOR_BOTTOM_LEFT(0.0f, 0.0f);
const Vec2 Vec2::ANCHOR_TOP_LEFT(0.0f, 1.0f);
const Vec2 Vec2::ANCHOR_BOTTOM_RIGHT(1.0f, 0.0f);
const Vec2 Vec2::ANCHOR_TOP_RIGHT(1.0f, 1.0f);
const Vec2 Vec2::ANCHOR_MIDDLE_RIGHT(1.0f, 0.5f);
const Vec2 Vec2::ANCHOR_MIDDLE_LEFT(0.0f, 0.5f);
const Vec2 Vec2::ANCHOR_MIDDLE_TOP(0.5f, 1.0f);
const Vec2 Vec2::ANCHOR_MIDDLE_BOTTOM(0.5f, 0.0f);

NS_END