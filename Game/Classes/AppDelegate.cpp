#include "AppDelegate.h"

USING_NS;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

void AppDelegate::initGLContextAttrs()
{

}

bool AppDelegate::applicationDidFinishLaunching()
{
    GLView* view = GLViewImpl::create("Hello GLView");

    while (!view->windowShouldClose())
    {
        view->swapBuffers();
        view->pollEvents();
    }
    view->end();

    return true;
}

void AppDelegate::applicationDidEnterBackground()
{

}

void AppDelegate::applicationWillEnterForeground()
{

}