#ifndef __PLATFORM_MACROS_H__
#define __PLATFORM_MACROS_H__

#include "platform/PlatformConfig.h"


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




#endif // !__PLATFORM_MACROS_H__