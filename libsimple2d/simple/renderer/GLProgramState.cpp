#include "renderer/GLProgramState.h"

#include <new>


GLProgramState* GLProgramState::create(GLProgram* glprogram)
{
    auto* ret = new (std::nothrow) GLProgramState();
    if (ret && ret->init(glprogram))
    {

        return ret;
    }
    return nullptr;
}

GLProgramState::GLProgramState()
    : _glprogram(nullptr)
{

}

GLProgramState::~GLProgramState()
{

}

bool GLProgramState::init(GLProgram* glprogram)
{
    _glprogram = glprogram;
    return true;
}


void GLProgramState::apply(const Mat4& modelView)
{

}
