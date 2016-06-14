#include "platform/GLView.h"

NS_BEGIN

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

void GLView::setViewName(const std::string& viewname)
{
    _viewName = viewname;
}

const std::string& GLView::getViewName() const
{
    return _viewName;
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