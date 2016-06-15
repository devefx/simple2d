#include "math/Mat4.h"

NS_BEGIN

inline const Mat4 Mat4::operator+(const Mat4& mat) const
{
    Mat4 result(*this);
    result.add(mat);
    return result;
}

inline Mat4& Mat4::operator+=(const Mat4& mat)
{
    add(mat);
    return *this;
}

inline const Mat4 Mat4::operator-(const Mat4& mat) const
{
    Mat4 result(*this);
    result.subtract(mat);
    return result;
}

inline Mat4& Mat4::operator-=(const Mat4& mat)
{
    subtract(mat);
    return *this;
}

inline const Mat4 Mat4::operator-() const
{
    Mat4 mat(*this);
    mat.negate();
    return mat;
}

inline const Mat4 Mat4::operator*(const Mat4& mat) const
{
    Mat4 result(*this);
    result.multiply(mat);
    return result;
}

inline Mat4& Mat4::operator*=(const Mat4& mat)
{
    multiply(mat);
    return *this;
}

NS_END