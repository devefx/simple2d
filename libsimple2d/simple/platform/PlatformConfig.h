#ifndef __PLATFORM_CONFIG_H__
#define __PLATFORM_CONFIG_H__

#define PLATFORM_UNKNOWN    0
#define PLATFORM_IOS        1
#define PLATFORM_ANDROID    2
#define PLATFORM_WIN32      3
#define PLATFORM_MAC        4
#define PLATFORM_LINUX      5

#define TARGET_PLATFORM     PLATFORM_MAC

// ios
#if defined(__APPLE__) && defined(TARGET_OS_IPHONE)
#undef  TARGET_PLATFORM
#define TARGET_PLATFORM     PLATFORM_IOS 
#endif

// android
#if defined(ANDROID) || defined(__ANDROID__)
#undef  TARGET_PLATFORM
#define TARGET_PLATFORM     PLATFORM_ANDROID
#endif

// win32
#if defined(_WIN32)|| defined(WIN32)
#undef  TARGET_PLATFORM
#define TARGET_PLATFORM     PLATFORM_WIN32
#endif

// mac
#if defined(__APPLE__) && defined(TARGET_OS_MAC)
#undef  TARGET_PLATFORM
#define TARGET_PLATFORM     PLATFORM_MAC
#endif

// linux
#if defined(__linux)
#undef  TARGET_PLATFORM
#define TARGET_PLATFORM     PLATFORM_LINUX
#endif

// check user set platform
#if !TARGET_PLATFORM
#error  "Cannot recognize the target platform; are you targeting an unsupported platform?"
#endif

#if TARGET_PLATFORM == PLATFORM_WIN32
#ifndef __MINGW32__
#pragma warning (disable:4127)
#endif // !__MINGW32__
#endif

#endif // !__PLATFORM_CONFIG_H__
