#ifndef __MAT4_H__
#define __MAT4_H__

#define DLL __declspec(dllexport)
#include "math/Vec3.h"

class DLL Mat4
{
public:
    float m[16];

    inline void transformPoint(const Vec3& point, Vec3* dst) const;
};


#endif