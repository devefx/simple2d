#ifndef __APPLICATION_ANDROID_H__
#define __APPLICATION_ANDROID_H__

#include "platform/PlatformConfig.h"

#if TARGET_PLATFORM == PLATFORM_ANDROID

#include "platform/ApplicationProtocol.h"

NS_BEGIN

class DLL Application : public ApplicationProtocol
{
public:

};

NS_END

#endif // TARGET_PLATFORM == PLATFORM_ANDROID

#endif // !__APPLICATION_ANDROID_H__
