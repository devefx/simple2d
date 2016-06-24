#include "base/Ref.h"

NS_BEGIN

Ref::Ref()
: _referenceCount(1)
{
}

Ref::~Ref()
{
}

void Ref::retain()
{
    ++_referenceCount;
}

void Ref::release()
{
    --_referenceCount;

    if (_referenceCount == 0)
    {

        delete this;
    }
}

Ref* Ref::autorelease()
{

    return this;
}

NS_END