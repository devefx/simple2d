#include "renderer/Renderer.h"

#include "renderer/Texture2D.h"

NS_BEGIN

static Texture2D* tex = new (std::nothrow) Texture2D;

Renderer::Renderer()
{

}

Renderer::~Renderer()
{

}

void Renderer::initGLView()
{
    tex->initWithData(nullptr, 0, Texture2D::PixelFormat::RGBA8888, 100, 100);
}

//

void Renderer::render()
{
    glBegin(GL_TRIANGLES);

    glColor3f(1.0, 0.0, 0.0);    // Red
    glVertex3f(0.0, 1.0, 0.0);

    glColor3f(0.0, 1.0, 0.0);    // Green
    glVertex3f(-1.0, -1.0, 0.0);

    glColor3f(0.0, 0.0, 1.0);    // Blue
    glVertex3f(1.0, -1.0, 0.0);

    glEnd();
}

void Renderer::clean()
{

}

void Renderer::clear()
{

}



NS_END