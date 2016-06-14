#ifndef __APPLICATION_IOS_H__
#define __APPLICATION_IOS_H__

#include "platform/PlatformConfig.h"

#if TARGET_PLATFORM == PLATFORM_IOS

#include "platform/ApplicationProtocol.h"

NS_BEGIN

class DLL Application : public ApplicationProtocol
{
public:

};

NS_END

#endif // TARGET_PLATFORM == PLATFORM_IOS

#endif // !__APPLICATION_IOS_H__
