#include "math/Mat4.h"

NS_BEGIN

Mat4::Mat4()
{
    *this = IDENTITY;
}

Mat4::Mat4(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24,
     float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44)
{
    set(m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44);
}

Mat4::Mat4(const float* mat)
{
    set(mat);
}

Mat4::Mat4(const Mat4& copy)
{
    memcpy(m, copy.m, MATRIX_SIZE);
}

Mat4::~Mat4()
{
}


void Mat4::add(const Mat4& mat)
{

}

static void add(const Mat4& m1, const Mat4& m2, Mat4* dst)
{

}

void Mat4::subtract(const Mat4& mat)
{

}

static void subtract(const Mat4& m1, const Mat4& m2, Mat4* dst)
{

}

void Mat4::multiply(const Mat4& mat)
{

}

static void multiply(const Mat4& m1, const Mat4& m2, Mat4* dst)
{

}

void Mat4::negate()
{

}


void Mat4::set(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24,
               float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44)
{
    m[0]  = m11; m[1]  = m21; m[2]  = m31; m[3]  = m41;
    m[4]  = m12; m[5]  = m22; m[6]  = m32; m[7]  = m42;
    m[8]  = m13; m[9]  = m23; m[10] = m33; m[11] = m43;
    m[12] = m14; m[13] = m24; m[14] = m34; m[15] = m44;
}

void Mat4::set(const float* mat)
{
    memcpy(this->m, mat, MATRIX_SIZE);
}

void Mat4::set(const Mat4& mat)
{
    memcpy(this->m, mat.m, MATRIX_SIZE);
}

const Mat4 Mat4::IDENTITY = Mat4(
                    1.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, 1.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f);

const Mat4 Mat4::ZERO = Mat4(
                    0, 0, 0, 0,
                    0, 0, 0, 0,
                    0, 0, 0, 0,
                    0, 0, 0, 0 );

NS_END