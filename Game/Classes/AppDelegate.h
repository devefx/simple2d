#ifndef __APP_DELEGATE_H__
#define __APP_DELEGATE_H__

#include "simple2d.h"

class AppDelegate : public simple2d::Application
{
public:
    AppDelegate();
    virtual ~AppDelegate();

    virtual void initGLContextAttrs();

    virtual bool applicationDidFinishLaunching();

    virtual void applicationDidEnterBackground();

    virtual void applicationWillEnterForeground();

};

#endif // !__APP_DELEGATE_H__
