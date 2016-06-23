#ifndef __PLATFORM_GL_H__
#define __PLATFORM_GL_H__

#include "platform/PlatformConfig.h"

#if TARGET_PLATFORM == PLATFORM_IOS
#include "platform/ios/GL-ios.h"
#elif TARGET_PLATFORM == PLATFORM_ANDROID
#include "platform/android/GL-android.h"
#elif TARGET_PLATFORM == PLATFORM_WIN32
#include "platform/win32/GL-win32.h"
#elif TARGET_PLATFORM == PLATFORM_MAC
#include "platform/mac/GL-mac.h"
#elif TARGET_PLATFORM == PLATFORM_LINUX
#include "platform/linux/GL-linux.h"
#endif

#endif // !__PLATFORM_GL_H__
