#ifndef __MATH_UTIL_H__
#define __MATH_UTIL_H__

#include "platform/PlatformMacros.h"
#include "math/Mat4.h"
#include "math/Vec3.h"

NS_BEGIN

/**
 * Defines a math utility class.
 *
 * This is primarily used for optimized internal math operations.
 */
class DLL MathUtil
{
    friend class Mat4;
    friend class Vec3;

public:

    /**
     * Updates the given scalar towards the given target using a smoothing function.
     * The given response time determines the amount of smoothing (lag). A longer
     * response time yields a smoother result and more lag. To force the scalar to
     * follow the target closely, provide a response time that is very small relative
     * to the given elapsed time.
     *
     * @param x the scalar to update.
     * @param target target value.
     * @param elapsedTime elapsed time between calls.
     * @param responseTime response time (in the same units as elapsedTime).
     */
    static void smooth(float* x, float target, float elapsedTime, float responseTime);

    /**
     * Updates the given scalar towards the given target using a smoothing function.
     * The given rise and fall times determine the amount of smoothing (lag). Longer
     * rise and fall times yield a smoother result and more lag. To force the scalar to
     * follow the target closely, provide rise and fall times that are very small relative
     * to the given elapsed time.
     *
     * @param x the scalar to update.
     * @param target target value.
     * @param elapsedTime elapsed time between calls.
     * @param riseTime response time for rising slope (in the same units as elapsedTime).
     * @param fallTime response time for falling slope (in the same units as elapsedTime).
     */
    static void smooth(float* x, float target, float elapsedTime, float riseTime, float fallTime);
    
    /**
     * Linearly interpolates between from value to to value by alpha which is in
     * the range [0,1]
     *
     * @param from the from value.
     * @param to the to value.
     * @param alpha the alpha value between [0,1]
     *
     * @return interpolated float value
     */
    static float lerp(float from, float to, float alpha);
private:
    //Indicates that if neon is enabled
    static bool isNeon32Enabled();
    static bool isNeon64Enabled();
private:

    static void addMatrix(const float* m, float scalar, float* dst);

    static void addMatrix(const float* m1, const float* m2, float* dst);

    static void subtractMatrix(const float* m1, const float* m2, float* dst);

    static void multiplyMatrix(const float* m, float scalar, float* dst);

    static void multiplyMatrix(const float* m1, const float* m2, float* dst);

    static void negateMatrix(const float* m, float* dst);

    static void transposeMatrix(const float* m, float* dst);

    static void transformVec4(const float* m, float x, float y, float z, float w, float* dst);

    static void transformVec4(const float* m, const float* v, float* dst);

    static void crossVec3(const float* v1, const float* v2, float* dst);

};

NS_END

#define MATRIX_SIZE ( sizeof(float) * 16)

#endif // !__MATH_UTIL_H__
