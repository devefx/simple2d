#ifndef __MATH_VEC2_H__
#define __MATH_VEC2_H__

#include <algorithm>
#include <functional>
#include <math.h>
#include "math/MathBase.h"

/**
 * @addtogroup math
 * @{
 */
NS_BEGIN

/** Clamp a value between from and to.
 */

inline float clampf(float value, float min_inclusive, float max_inclusive)
{
    if (min_inclusive > max_inclusive) {
        std::swap(min_inclusive, max_inclusive);
    }
    return value < min_inclusive ? min_inclusive : value < max_inclusive? value : max_inclusive;
}

class DLL Vec2
{
public:
    float x;
    float y;

    Vec2();

    Vec2(float x, float y);

    Vec2(const Vec2& copy);

    ~Vec2();

    inline bool isZero() const;

    inline bool isOne() const;

    //

    inline void set(float xx, float yy);

    inline void set(const Vec2& v);

public:
    bool equals(const Vec2& target) const;

    /** equals to Vec2(0,0) */
    static const Vec2 ZERO;
    /** equals to Vec2(1,1) */
    static const Vec2 ONE;
    /** equals to Vec2(1,0) */
    static const Vec2 UNIT_X;
    /** equals to Vec2(0,1) */
    static const Vec2 UNIT_Y;
    /** equals to Vec2(0.5, 0.5) */
    static const Vec2 ANCHOR_MIDDLE;
    /** equals to Vec2(0, 0) */
    static const Vec2 ANCHOR_BOTTOM_LEFT;
    /** equals to Vec2(0, 1) */
    static const Vec2 ANCHOR_TOP_LEFT;
    /** equals to Vec2(1, 0) */
    static const Vec2 ANCHOR_BOTTOM_RIGHT;
    /** equals to Vec2(1, 1) */
    static const Vec2 ANCHOR_TOP_RIGHT;
    /** equals to Vec2(1, 0.5) */
    static const Vec2 ANCHOR_MIDDLE_RIGHT;
    /** equals to Vec2(0, 0.5) */
    static const Vec2 ANCHOR_MIDDLE_LEFT;
    /** equals to Vec2(0.5, 1) */
    static const Vec2 ANCHOR_MIDDLE_TOP;
    /** equals to Vec2(0.5, 0) */
    static const Vec2 ANCHOR_MIDDLE_BOTTOM;
};

typedef Vec2 Point;

NS_END
/**
 end of math group
 @}
 */

#include "math/Vec2.inl"

#endif // !__MATH_VEC2_H__
