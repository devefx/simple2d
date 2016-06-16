#ifndef __APPLICATION_LINUX_H__
#define __APPLICATION_LINUX_H__

#include "platform/PlatformConfig.h"

#if TARGET_PLATFORM == PLATFORM_LINUX

#include "platform/ApplicationProtocol.h"

NS_BEGIN

class DLL Application : public ApplicationProtocol
{
public:
    Application();

    virtual ~Application();

    int run();

    static Application* getInstance();

    void setAnimationInterval(double interval);

    virtual LanguageType getCurrentLanguage();

    virtual const char * getCurrentLanguageCode();

    virtual Platform getTargetPlatform();

    virtual bool openURL(const std::string &url);
protected:
    long       _animationInterval;

    static Application * sm_pSharedApplication;
};

NS_END

#endif // TARGET_PLATFORM == PLATFORM_LINUX

#endif // !__APPLICATION_LINUX_H__
