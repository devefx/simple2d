#ifndef __PLATFORM_MACROS_H__
#define __PLATFORM_MACROS_H__

#include "platform/PlatformConfig.h"
#include "platform/PlatformDefine.h"

/// @name namespace simple2d
/// @{
#ifdef __cplusplus
#define NS_BEGIN                     namespace simple2d {
#define NS_END                       }
#define USING_NS                     using namespace simple2d
#define NS                           ::simple2d
#else
#define NS_BEGIN 
#define NS_END 
#define USING_NS 
#define NS
#endif 
//  end of namespace group
/// @}





#define SAFE_DELETE(p)           do { delete (p); (p) = nullptr; } while(0)
#define SAFE_DELETE_ARRAY(p)     do { if(p) { delete[] (p); (p) = nullptr; } } while(0)
#define SAFE_FREE(p)             do { if(p) { free(p); (p) = nullptr; } } while(0)
#define SAFE_RELEASE(p)          do { if(p) { (p)->release(); } } while(0)
#define SAFE_RELEASE_NULL(p)     do { if(p) { (p)->release(); (p) = nullptr; } } while(0)
#define SAFE_RETAIN(p)           do { if(p) { (p)->retain(); } } while(0)
#define BREAK_IF(cond)           if(cond) break



#ifndef SIMPLE2D_DEBUG
#define LOG(...)       do {} while (0)
#define LOGINFO(...)   do {} while (0)
#define LOGERROR(...)  do {} while (0)
#define LOGWARN(...)   do {} while (0)
#else// FIXME
#define LOG(...)       do {} while (0)
#define LOGINFO(...)   do {} while (0)
#define LOGERROR(...)  do {} while (0)
#define LOGWARN(...)   do {} while (0)
#endif // !SIMPLE2D_DEBUG



#endif // !__PLATFORM_MACROS_H__