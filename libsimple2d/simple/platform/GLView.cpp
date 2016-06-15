#include "platform/GLView.h"

NS_BEGIN

//default context attributions are setted as follows
GLContextAttrs GLView::_glContextAttrs = {5, 6, 5, 0, 16, 0};

void GLView::setGLContextAttrs(GLContextAttrs& glContextAttrs)
{
    _glContextAttrs = glContextAttrs;
}

GLContextAttrs GLView::getGLContextAttrs()
{
    return _glContextAttrs;
}

GLView::GLView()
: _scaleX(1.0f)
, _scaleY(1.0f)
, _resolutionPolicy(ResolutionPolicy::UNKNOWN)
{
}

GLView::~GLView()
{

}

void GLView::pollEvents()
{

}

void GLView::updateDesignResolutionSize()
{

}

void GLView::setDesignResolutionSize(float width, float height, ResolutionPolicy resolutionPolicy)
{

}

const Size& GLView::getDesignResolutionSize() const
{
    return _designResolutionSize;
}

const Size& GLView::getFrameSize() const
{
    return _screenSize;
}

void GLView::setFrameSize(float width, float height)
{
    _designResolutionSize = _screenSize = Size(width, height);
}

Rect GLView::getVisibleRect() const
{
    Rect rect;
    rect.origin = getVisibleOrigin();
    rect.size = getVisibleSize();
    return rect;
}

Size GLView::getVisibleSize() const
{
    if (_resolutionPolicy == ResolutionPolicy::NO_BORDER)
    {
        return Size(_screenSize.width / _scaleX, _screenSize.height / _scaleY);
    }
    return _designResolutionSize;
}

Vec2 GLView::getVisibleOrigin() const
{
    if (_resolutionPolicy == ResolutionPolicy::NO_BORDER)
    {
        return Vec2((_designResolutionSize.width - _screenSize.width / _scaleX) / 2.0f,
                    (_designResolutionSize.height - _screenSize.height / _scaleY) / 2.0f);
    }
    return Vec2::ZERO;
}

void GLView::setViewPortInPoints(float x , float y , float w , float h)
{
    glViewport((GLint)(x * _scaleX + _viewPortRect.origin.x),
               (GLint)(y * _scaleY + _viewPortRect.origin.y),
               (GLsizei)(w * _scaleX),
               (GLsizei)(h * _scaleY));
}

void GLView::setScissorInPoints(float x , float y , float w , float h)
{
    glScissor((GLint)(x * _scaleX + _viewPortRect.origin.x),
              (GLint)(y * _scaleY + _viewPortRect.origin.y),
              (GLsizei)(w * _scaleX),
              (GLsizei)(h * _scaleY));
}

bool GLView::isScissorEnabled()
{
    return GL_TRUE == glIsEnabled(GL_SCISSOR_TEST);
}

Rect GLView::getScissorRect() const
{
    GLfloat params[4];
    glGetFloatv(GL_SCISSOR_BOX, params);
    float x = (params[0] - _viewPortRect.origin.x) / _scaleX;
    float y = (params[1] - _viewPortRect.origin.y) / _scaleY;
    float w = params[2] / _scaleX;
    float h = params[3] / _scaleY;
    return Rect(x, y, w, h);
}

void GLView::setViewName(const std::string& viewname)
{
    _viewName = viewname;
}

const std::string& GLView::getViewName() const
{
    return _viewName;
}

void GLView::handleTouchesBegin(int num, intptr_t ids[], float xs[], float ys[])
{

}

void GLView::handleTouchesMove(int num, intptr_t ids[], float xs[], float ys[])
{

}

void GLView::handleTouchesEnd(int num, intptr_t ids[], float xs[], float ys[])
{

}

void GLView::handleTouchesCancel(int num, intptr_t ids[], float xs[], float ys[])
{

}

const Rect& GLView::getViewPortRect() const
{
    return _viewPortRect;
}

float GLView::getScaleX() const
{
    return _scaleX;
}

float GLView::getScaleY() const
{
    return _scaleY;
}

NS_END