#ifndef __MATH_VEC3_H__
#define __MATH_VEC3_H__

#if defined(__APPLE__)
#define DLL __attribute__ ((visibility("default")))
#elif defined(_WIN32)
#ifdef _USRDLL
#define DLL __declspec(dllexport)
#else
#define DLL __declspec(dllimport)
#endif // _USRDLL


#endif



class DLL Vec3
{
public:
    /**
     * The x-coordinate.
     */
    float x;

    /**
     * The y-coordinate.
     */
    float y;

    /**
     * The z-coordinate.
     */
    float z;
};


#endif // !__MATH_VEC3_H__
