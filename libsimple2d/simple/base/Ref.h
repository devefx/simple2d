#ifndef __BASE_REF_H__
#define __BASE_REF_H__

#include "platform/PlatformMacros.h"

/**
 * @addtogroup base
 * @{
 */
NS_BEGIN

class DLL Ref
{
public:

    void retain();

    void release();

    Ref* autorelease();

    unsigned int getReferenceCount() const;

protected:
    Ref();

public:
    virtual ~Ref();

protected:

    unsigned int _referenceCount;

    friend class AutoreleasePool;
};

NS_END
// end of base group
/** @} */

#endif // !__BASE_REF_H__
