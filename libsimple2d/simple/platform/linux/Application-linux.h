#ifndef __APPLICATION_LINUX_H__
#define __APPLICATION_LINUX_H__

#include "platform/PlatformConfig.h"

#if TARGET_PLATFORM == PLATFORM_LINUX

#include "platform/ApplicationProtocol.h"

NS_BEGIN

class DLL Application : public ApplicationProtocol
{
public:

};

NS_END

#endif // TARGET_PLATFORM == PLATFORM_LINUX

#endif // !__APPLICATION_LINUX_H__
