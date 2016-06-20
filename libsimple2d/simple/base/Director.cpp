#include "base/Director.h"

#include "base/AutoreleasePool.h"
#include "platform/Application.h"

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
    setDefaultValues();

    _purgeDirectorInNextLoop = false;

    _restartDirectorInNextLoop = false;

    _matrix = Matrix::getInstance();

    _openGLView = nullptr;

    
    _renderer = new (std::nothrow) Renderer;

    return true;
}

Director::~Director()
{
    SAFE_RELEASE(s_SharedDirector);
   
    s_SharedDirector = nullptr;
}

void Director::setDefaultValues(void)
{

}

void Director::setGLDefaultValues()
{

}

void Director::drawScene()
{
    //

    if (_openGLView)
    {
        _openGLView->pollEvents();
    }

    //

    _renderer->clear();

    //

    _renderer->render();

    //

    if (_openGLView)
    {
        _openGLView->swapBuffers();
    }

    //
}

//

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

        //

        _isStatusLabelUpdated = true;

        //

        _renderer->initGLView();

        //
    }
}

//

void Director::setAlphaBlending(bool on)
{

}

void Director::setDepthTest(bool on)
{

}

//

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
        drawScene();

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