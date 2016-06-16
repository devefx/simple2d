#include "base/Matrix.h"

NS_BEGIN

static Matrix *s_SharedMatrix = nullptr;

Matrix::Matrix()
{
}

Matrix::~Matrix()
{
}

Matrix* Matrix::getInstance()
{
    if (s_SharedMatrix == nullptr)
    {
        s_SharedMatrix = new (std::nothrow) Matrix();
        s_SharedMatrix->initMatrixStack();
    }
    return s_SharedMatrix;
}

void Matrix::initMatrixStack()
{
    while (!_modelViewMatrixStack.empty())
    {
        _modelViewMatrixStack.pop();
    }

    while (!_projectionMatrixStack.empty())
    {
        _projectionMatrixStack.pop();
    }

    while (!_textureMatrixStack.empty())
    {
        _textureMatrixStack.pop();
    }

    _modelViewMatrixStack.push(Mat4::IDENTITY);
    _projectionMatrixStack.push(Mat4::IDENTITY);
    _textureMatrixStack.push(Mat4::IDENTITY);
}

void Matrix::pushMatrix(MATRIX_STACK_TYPE type)
{
    if(type == MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW)
    {
        _modelViewMatrixStack.push(_modelViewMatrixStack.top());
    }
    else if(type == MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION)
    {
        _projectionMatrixStack.push(_projectionMatrixStack.top());
    }
    else if(type == MATRIX_STACK_TYPE::MATRIX_STACK_TEXTURE)
    {
        _textureMatrixStack.push(_textureMatrixStack.top());
    }
}

void Matrix::popMatrix(MATRIX_STACK_TYPE type)
{
    if(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW == type)
    {
        _modelViewMatrixStack.pop();
    }
    else if(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION == type)
    {
        _projectionMatrixStack.pop();
    }
    else if(MATRIX_STACK_TYPE::MATRIX_STACK_TEXTURE == type)
    {
        _textureMatrixStack.pop();
    }
}

void Matrix::loadIdentityMatrix(MATRIX_STACK_TYPE type)
{
    if(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW == type)
    {
        _modelViewMatrixStack.top() = Mat4::IDENTITY;
    }
    else if(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION == type)
    {
        _projectionMatrixStack.top() = Mat4::IDENTITY;
    }
    else if(MATRIX_STACK_TYPE::MATRIX_STACK_TEXTURE == type)
    {
        _textureMatrixStack.top() = Mat4::IDENTITY;
    }
}

void Matrix::loadMatrix(MATRIX_STACK_TYPE type, const Mat4& mat)
{
    if(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW == type)
    {
        _modelViewMatrixStack.top() = mat;
    }
    else if(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION == type)
    {
        _projectionMatrixStack.top() = mat;
    }
    else if(MATRIX_STACK_TYPE::MATRIX_STACK_TEXTURE == type)
    {
        _textureMatrixStack.top() = mat;
    }
}

void Matrix::multiplyMatrix(MATRIX_STACK_TYPE type, const Mat4& mat)
{
    if(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW == type)
    {
        _modelViewMatrixStack.top() *= mat;
    }
    else if(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION == type)
    {
        _projectionMatrixStack.top() *= mat;
    }
    else if(MATRIX_STACK_TYPE::MATRIX_STACK_TEXTURE == type)
    {
        _textureMatrixStack.top() *= mat;
    }
}

const Mat4& Matrix::getMatrix(MATRIX_STACK_TYPE type)
{
    if(type == MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW)
    {
        return _modelViewMatrixStack.top();
    }
    else if(type == MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION)
    {
        return _projectionMatrixStack.top();
    }
    else if(type == MATRIX_STACK_TYPE::MATRIX_STACK_TEXTURE)
    {
        return _textureMatrixStack.top();
    }
    return  _modelViewMatrixStack.top();
}

void Matrix::resetMatrixStack()
{
    initMatrixStack();
}


NS_END