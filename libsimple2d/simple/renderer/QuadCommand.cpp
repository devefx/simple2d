#include "renderer/QuadCommand.h"

#include "renderer/GLStateCache.h"


QuadCommand::QuadCommand()
    : _materialID(0)
    , _textureID(0)
    , _blendType(BlendFunc::DISABLE)
    , _quads(nullptr)
    , _quadsCount(0)
{

}

RenderCommand::Type QuadCommand::getType() const 
{
    return RenderCommand::Type::QUAD_COMMAND;
}

void QuadCommand::generateMaterialID()
{

}

void QuadCommand::useMaterial() const
{
    GL::bindTexture2D(_textureID);

    GL::blendFunc(_blendType.src, _blendType.dst);
}

