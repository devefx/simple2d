#ifndef __APPLICATION_PROTOCOL_H__
#define __APPLICATION_PROTOCOL_H__

#include "platform/PlatformMacros.h"
#include "base/AutoreleasePool.h"
#include "platform/Common.h"

NS_BEGIN
/**
 * @addtogroup platform
 * @{
 */

class DLL ApplicationProtocol
{
public:
    enum class Platform
    {
        OS_IPHONE,
        OS_IPAD,
        OS_ANDROID,
        OS_WINDOWS,
        OS_MAC,
        OS_LINUX
    };

    virtual ~ApplicationProtocol(){
        PoolManager::destroyInstance();
    }

    virtual bool applicationDidFinishLaunching() = 0;

    virtual void applicationDidEnterBackground() = 0;

    virtual void applicationWillEnterForeground() = 0;

    virtual void setAnimationInterval(double interval) = 0;

    virtual void initGLContextAttrs() {}

    virtual LanguageType getCurrentLanguage() = 0;

    virtual const char * getCurrentLanguageCode() = 0;

    virtual Platform getTargetPlatform() = 0;

    virtual bool openURL(const std::string &url) = 0;

};

NS_END
// end of platform group
/** @} */

#endif // !__APPLICATION_PROTOCOL_H__
