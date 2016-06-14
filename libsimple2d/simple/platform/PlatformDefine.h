#ifndef __PLATFORM_DEFINE_H__
#define __PLATFORM_DEFINE_H__

#include "platform/PlatformConfig.h"


#if TARGET_PLATFORM == PLATFORM_IOS
#include "platform/ios/PlatformDefine-ios.h"
#elif TARGET_PLATFORM == PLATFORM_ANDROID
#include "platform/android/PlatformDefine-android.h"
#elif TARGET_PLATFORM == PLATFORM_WIN32
#include "platform/win32/PlatformDefine-win32.h"
#elif TARGET_PLATFORM == PLATFORM_MAC
#include "platform/mac/PlatformDefine-mac.h"
#elif TARGET_PLATFORM == PLATFORM_LINUX
#include "platform/linux/PlatformDefine-linux.h"
#endif


#endif // !__PLATFORM_DEFINE_H__
