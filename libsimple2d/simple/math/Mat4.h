#ifndef __MATH_MAT4_H__
#define __MATH_MAT4_H__

#include "math/Vec3.h"
#include "math/Vec4.h"


#define MATRIX_SIZE (sizeof(float) * 16)

/**
 * @addtogroup math
 * @{
 */
NS_BEGIN

class DLL Mat4
{
public:
    float m[16];

    Mat4();

    Mat4(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24,
        float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44);

    Mat4(const float* mat);

    Mat4(const Mat4& copy);

    ~Mat4();


    void add(const Mat4& mat);

    static void add(const Mat4& m1, const Mat4& m2, Mat4* dst);

    void subtract(const Mat4& mat);

    static void subtract(const Mat4& m1, const Mat4& m2, Mat4* dst);

    void multiply(const Mat4& mat);

    static void multiply(const Mat4& m1, const Mat4& m2, Mat4* dst);

    void negate();




    void set(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24,
        float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44);

    void set(const float* mat);

    void set(const Mat4& mat);


public:
    inline const Mat4 operator+(const Mat4& mat) const;

    inline Mat4& operator+=(const Mat4& mat);

    inline const Mat4 operator-(const Mat4& mat) const;

    inline Mat4& operator-=(const Mat4& mat);

    inline const Mat4 operator-() const;

    inline const Mat4 operator*(const Mat4& mat) const;

    inline Mat4& operator*=(const Mat4& mat);

    /** equals to a matrix full of zeros */
    static const Mat4 ZERO;
    /** equals to the identity matrix */
    static const Mat4 IDENTITY;
};

NS_END
/**
 end of math group
 @}
 */

#include "math/Mat4.inl"

#endif // !__MATH_MAT4_H__
