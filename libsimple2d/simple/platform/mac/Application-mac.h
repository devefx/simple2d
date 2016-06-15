#ifndef __APPLICATION_MAC_H__
#define __APPLICATION_MAC_H__

#include "platform/PlatformConfig.h"

#if TARGET_PLATFORM == PLATFORM_MAC

#include "platform/ApplicationProtocol.h"

NS_BEGIN

class DLL Application : public ApplicationProtocol
{
public:
    Application();

    virtual ~Application();

    virtual void setAnimationInterval(double interval);

    int run();

    static Application* getInstance();

    virtual LanguageType getCurrentLanguage();

    virtual const char * getCurrentLanguageCode();

    virtual Platform getTargetPlatform();

    virtual bool openURL(const std::string &url);

protected:
    long _animationInterval;

    static Application * sm_pSharedApplication;
};

NS_END

#endif // TARGET_PLATFORM == PLATFORM_MAC

#endif // !__APPLICATION_MAC_H__
