#ifndef __APPLICATION_WIN32_H__
#define __APPLICATION_WIN32_H__

#include "platform/PlatformConfig.h"

#if TARGET_PLATFORM == PLATFORM_WIN32

#include <windows.h>

#include "platform/ApplicationProtocol.h"

NS_BEGIN

class DLL Application : public ApplicationProtocol
{
public:
    Application();

    virtual ~Application();

    int run();

    static Application* getInstance();

    /* override functions */
    virtual void setAnimationInterval(double interval);
    virtual LanguageType getCurrentLanguage();

    virtual const char * getCurrentLanguageCode();

    virtual Platform getTargetPlatform();

    virtual bool openURL(const std::string &url);

protected:
    HINSTANCE           _instance;
    LARGE_INTEGER       _animationInterval;

    static Application * sm_pSharedApplication;
};

NS_END

#endif // TARGET_PLATFORM == PLATFORM_WIN32

#endif // !__APPLICATION_WIN32_H__
