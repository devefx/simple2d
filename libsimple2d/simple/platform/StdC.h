#ifndef __STDC_H__
#define __STDC_H__

#include "platform/PlatformMacros.h"


#if TARGET_PLATFORM == PLATFORM_IOS
#include "platform/ios/StdC-ios.h"
#elif TARGET_PLATFORM == PLATFORM_ANDROID
#include "platform/android/StdC-android.h"
#elif TARGET_PLATFORM == PLATFORM_WIN32
#include "platform/win32/StdC-win32.h"
#elif TARGET_PLATFORM == PLATFORM_MAC
#include "platform/mac/StdC-mac.h"
#elif TARGET_PLATFORM == PLATFORM_LINUX
#include "platform/linux/StdC-linux.h"
#endif



#endif // __STDC_H__
