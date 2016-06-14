#ifndef __APPLICATION_MAC_H__
#define __APPLICATION_MAC_H__

#include "platform/PlatformConfig.h"

#if TARGET_PLATFORM == PLATFORM_MAC

#include "platform/ApplicationProtocol.h"

NS_BEGIN

class DLL Application : public ApplicationProtocol
{
public:

};

NS_END

#endif // TARGET_PLATFORM == PLATFORM_MAC

#endif // !__APPLICATION_MAC_H__
