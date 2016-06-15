#include "AppDelegate.h"

USING_NS;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

void AppDelegate::initGLContextAttrs()
{
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching()
{
    auto director = Director::getInstance();

    auto glview = director->getOpenGLView();

    if (!glview)
    {
        glview = GLViewImpl::create("Hello GLView");

        director->setOpenGLView(glview);
    }

    director->setAnimationInterval(1.0 / 60);

    return true;
}

void AppDelegate::applicationDidEnterBackground()
{

}

void AppDelegate::applicationWillEnterForeground()
{

}