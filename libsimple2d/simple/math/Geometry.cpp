#include "math/Geometry.h"

NS_BEGIN

Size::Size()
: width(0)
, height(0)
{
}

Size::Size(float w, float h)
: width(w)
, height(h)
{
}

Size::Size(const Size& other)
: width(other.width)
, height(other.height)
{
}

void Size::setSize(float width, float height)
{
    this->width = width;
    this->height = height;
}

bool Size::equals(const Size& target) const
{
    return false;
}

const Size Size::ZERO = Size(0, 0);

//-------------------------------------------------------------------
//
// Rect
//
//-------------------------------------------------------------------
Rect::Rect(void)
{
    setRect(0.0f, 0.0f, 0.0f, 0.0f);
}

Rect::Rect(float x, float y, float width, float height)
{
    setRect(x, y, width, height);
}

Rect::Rect(const Rect& other)
{
    setRect(other.origin.x, other.origin.y, other.size.width, other.size.height);
}

void Rect::setRect(float x, float y, float width, float height)
{
    origin.x = x;
    origin.y = y;
    size.width = width;
    size.height = height;
}

float Rect::getMinX() const
{
    return origin.x;
}

float Rect::getMidX() const
{
    return getMaxX() / 2.0f;
}

float Rect::getMaxX() const
{
    return origin.x + size.width;
}

float Rect::getMinY() const
{
    return origin.y;
}

float Rect::getMidY() const
{
    return getMaxY() / 2.0f;
}

float Rect::getMaxY() const
{
    return origin.y + size.height;
}

bool Rect::equals(const Rect& rect) const
{
    return (origin.equals(rect.origin) && 
        size.equals(rect.size));
}

bool Rect::containsPoint(const Vec2& point) const
{
    return point.x >= getMinX() && point.x <= getMaxX() &&
        point.y >= getMinY() && point.y <= getMaxY();
}

bool Rect::intersectsRect(const Rect& rect) const
{
    return !(    getMaxX() < rect.getMinX() ||
            rect.getMaxX() <      getMinX() ||
                 getMaxY() < rect.getMinY() ||
            rect.getMaxY() <      getMinY());
}

bool Rect::intersectsCircle(const Vec2& center, float radius) const
{
    // TODO
    return true;
}

Rect Rect::unionWithRect(const Rect & rect) const
{
    return Rect::ZERO;
}

void Rect::merge(const Rect& rect)
{
    // TODO
}

const Rect Rect::ZERO = Rect(0.0f, 0.0f, 0.0f, 0.0f);

NS_END