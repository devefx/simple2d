#ifndef __SIMPLE_H__
#define __SIMPLE_H__

// base
#include "base/Ref.h"

// platfomr
#include "platform/PlatformConfig.h"
#include "platform/PlatformMacros.h"

#include "platform/Application.h"
#include "platform/GL.h"

#if TARGET_PLATFORM == PLATFORM_IOS

#elif TARGET_PLATFORM == PLATFORM_ANDROID

#elif TARGET_PLATFORM == PLATFORM_WIN32 || TARGET_PLATFORM == PLATFORM_MAC || TARGET_PLATFORM == PLATFORM_LINUX
    #include "platform/desktop/GLViewImpl-desktop.h"
#endif


#endif