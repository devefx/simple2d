#ifndef __MATH_VEC3_H__
#define __MATH_VEC3_H__

#include "math/MathBase.h"

/**
 * @addtogroup math
 * @{
 */
NS_BEGIN

class DLL Vec3
{
public:
    float x;
    float y;
    float z;

    Vec3();

    Vec3(float xx, float yy, float zz);

    Vec3(const Vec3& copy);

    ~Vec3();

    inline bool isZero() const;

    inline bool isOne() const;

    //

    inline void set(float xx, float yy, float zz);

    inline void set(const Vec3& v);

public:

    /** equals to Vec3(0,0,0) */
    static const Vec3 ZERO;
    /** equals to Vec3(1,1,1) */
    static const Vec3 ONE;
    /** equals to Vec3(1,0,0) */
    static const Vec3 UNIT_X;
    /** equals to Vec3(0,1,0) */
    static const Vec3 UNIT_Y;
    /** equals to Vec3(0,0,1) */
    static const Vec3 UNIT_Z;
};

NS_END
/**
 end of math group
 @}
 */

#include "math/Vec3.inl"

#endif // !__MATH_VEC3_H__
