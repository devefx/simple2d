#ifndef __PLATFORM_APPLICATION_H__
#define __PLATFORM_APPLICATION_H__

#include "platform/PlatformConfig.h"


#if TARGET_PLATFORM == PLATFORM_IOS
#include "platform/ios/Application-ios.h"
#elif TARGET_PLATFORM == PLATFORM_ANDROID
#include "platform/android/Application-android.h"
#elif TARGET_PLATFORM == PLATFORM_WIN32
#include "platform/win32/Application-win32.h"
#elif TARGET_PLATFORM == PLATFORM_MAC
#include "platform/mac/Application-mac.h"
#elif TARGET_PLATFORM == PLATFORM_LINUX
#include "platform/linux/Application-linux.h"
#endif


#endif // !__PLATFORM_APPLICATION_H__
