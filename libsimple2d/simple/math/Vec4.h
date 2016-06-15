#ifndef __MATH_VEC4_H__
#define __MATH_VEC4_H__

#include "math/MathBase.h"

/**
 * @addtogroup math
 * @{
 */
NS_BEGIN

class DLL Vec4
{
public:
    float x;
    float y;
    float z;
    float w;

    Vec4();

    Vec4(float xx, float yy, float zz, float ww);

    Vec4(const Vec4& copy);

    ~Vec4();

    bool isZero() const;

    bool isOne() const;

    //

    void set(float xx, float yy, float zz, float ww);

    void set(const Vec4& v);

public:

    /** equals to Vec4(0,0,0,0) */
    static const Vec4 ZERO;
    /** equals to Vec4(1,1,1,1) */
    static const Vec4 ONE;
    /** equals to Vec4(1,0,0,0) */
    static const Vec4 UNIT_X;
    /** equals to Vec4(0,1,0,0) */
    static const Vec4 UNIT_Y;
    /** equals to Vec4(0,0,1,0) */
    static const Vec4 UNIT_Z;
    /** equals to Vec4(0,0,0,1) */
    static const Vec4 UNIT_W;
};

NS_END
/**
 end of math group
 @}
 */

#include "math/Vec4.inl"

#endif // !__MATH_VEC4_H__
