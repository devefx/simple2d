#ifndef __GLPROGRAM_H__
#define __GLPROGRAM_H__

#if defined(__APPLE__)
#define DLL __attribute__ ((visibility("default")))
#elif defined(_WIN32)
#ifdef _USRDLL
#define DLL __declspec(dllexport)
#else
#define DLL __declspec(dllimport)
#endif
#endif

class DLL GLProgram
{

public:
    /**Enum the preallocated vertex attribute. */
    enum
    {
        /**Index 0 will be used as Position.*/
        VERTEX_ATTRIB_POSITION,
        /**Index 1 will be used as Color.*/
        VERTEX_ATTRIB_COLOR,
        /**Index 2 will be used as Tex coord unit 0.*/
        VERTEX_ATTRIB_TEX_COORD,
        /**Index 3 will be used as Tex coord unit 1.*/
        VERTEX_ATTRIB_TEX_COORD1,
        /**Index 4 will be used as Tex coord unit 2.*/
        VERTEX_ATTRIB_TEX_COORD2,
        /**Index 5 will be used as Tex coord unit 3.*/
        VERTEX_ATTRIB_TEX_COORD3,
        /**Index 6 will be used as Normal.*/
        VERTEX_ATTRIB_NORMAL,
        /**Index 7 will be used as Blend weight for hardware skin.*/
        VERTEX_ATTRIB_BLEND_WEIGHT,
        /**Index 8 will be used as Blend index.*/
        VERTEX_ATTRIB_BLEND_INDEX,
        VERTEX_ATTRIB_MAX,

        // backward compatibility
        VERTEX_ATTRIB_TEX_COORDS = VERTEX_ATTRIB_TEX_COORD,
    };

};

#endif // !__GLPROGRAM_H__



