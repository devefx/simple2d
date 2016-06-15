#include "base/Director.h"

#include "base/AutoreleasePool.h"
#include "platform/Application.h"

using namespace std;

NS_BEGIN

#define defaultFPS  60

static DisplayLinkDirector *s_SharedDirector = nullptr;

Director* Director::getInstance()
{
    if (s_SharedDirector == nullptr)
    {
        s_SharedDirector = new (std::nothrow) DisplayLinkDirector();
        s_SharedDirector->init();
    }
    return s_SharedDirector;
}

Director::Director()
: _isStatusLabelUpdated(true)
{
}

bool Director::init()
{

    _purgeDirectorInNextLoop = false;

    _restartDirectorInNextLoop = false;

    _matrixControl = new MatrixControl();

    _openGLView = nullptr;

    return true;
}

Director::~Director()
{
    SAFE_RELEASE(s_SharedDirector);
   
    s_SharedDirector = nullptr;
}

void Director::setOpenGLView(GLView *openGLView)
{
    if (_openGLView != openGLView)
    {
        if (_openGLView)
        {
            _openGLView->release();
        }
        _openGLView = openGLView;
        _openGLView->retain();


        _isStatusLabelUpdated = true;

    }
}

void Director::end()
{
    _purgeDirectorInNextLoop = true;
}

void Director::restart()
{
    _restartDirectorInNextLoop = true;
}

void Director::reset()
{

}

void Director::purgeDirector()
{
    reset();


    if (_openGLView)
    {
        _openGLView->end();
        _openGLView = nullptr;
    }

    release();
}

void Director::restartDirector()
{
    reset();


}

void Director::pause()
{

}

void Director::resume()
{

}


//-------------------------------------------------------------------------
//
// DisplayLinkDirector
//
//-------------------------------------------------------------------------

void DisplayLinkDirector::startAnimation()
{
    _invalid = false;

    Application::getInstance()->setAnimationInterval(_animationInterval);


}

void DisplayLinkDirector::stopAnimation()
{
    _invalid = true;
}

void DisplayLinkDirector::mainLoop()
{
    if (_purgeDirectorInNextLoop)
    {
        _purgeDirectorInNextLoop = false;
        purgeDirector();
    }
    else if (_restartDirectorInNextLoop)
    {
        _restartDirectorInNextLoop = false;
        restartDirector();
    }
    else if (!_invalid)
    {

        PoolManager::getInstance()->getCurrentPool()->clear();
    }
}

void DisplayLinkDirector::setAnimationInterval(double interval)
{
    _animationInterval = interval;
    if (!_invalid)
    {
        stopAnimation();
        startAnimation();
    }
}


NS_END