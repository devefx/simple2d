#ifndef __MATH_GEOMETRY_H__
#define __MATH_GEOMETRY_H__

#include <math.h>

#include "platform/PlatformMacros.h"
#include "math/Math.h"

/**
 * @addtogroup math
 * @{
 */
NS_BEGIN

class DLL Size
{
public:
    float width;
    float height;
public:
    
public:
    Size();
    Size(float width, float height);
    Size(const Size& other);
    
    void setSize(float width, float height);

    bool equals(const Size& target) const;

    static const Size ZERO;
};

class DLL Rect
{
public:
    Vec2 origin;

    Size size;

public:
    Rect();

    Rect(float x, float y, float width, float height);

    Rect(const Rect& other);

    void setRect(float x, float y, float width, float height);

    float getMinX() const;

    float getMidX() const;

    float getMaxX() const;

    float getMinY() const;

    float getMidY() const;

    float getMaxY() const;

    bool equals(const Rect& rect) const;

    bool containsPoint(const Vec2& point) const;

    bool intersectsRect(const Rect& rect) const;

    bool intersectsCircle(const Vec2& center, float radius) const;

    Rect unionWithRect(const Rect & rect) const;

    void merge(const Rect& rect);

    static const Rect ZERO;
};

NS_END
/**
 end of math group
 @}
 */

#endif // !__MATH_GEOMETRY_H__
