#include "renderer/QuadCommand.h"

#include "renderer/GLStateCache.h"
#include "xxhash.h"

NS_BEGIN

QuadCommand::QuadCommand()
    : _materialID(0)
    , _textureID(0)
    , _blendType(BlendFunc::DISABLE)
    , _quads(nullptr)
    , _quadsCount(0)
{

}

void QuadCommand::init(float globalOrder, GLuint textureID, GLProgramState* shader, const BlendFunc& blendType, V3F_C4B_T2F_Quad* quads,
                       size_t quadCount, const Mat4& mv)
{
    RenderCommand::init(globalOrder);

    _quadsCount = quadCount;
    _quads = quads;

    _mv = mv;

    if (_textureID != textureID || _blendType.src != blendType.src || _blendType.dst != blendType.dst || _glProgramState != shader)
    {
        _textureID = textureID;
        _blendType = blendType;
        _glProgramState = shader;

        generateMaterialID();
    }
}

void QuadCommand::generateMaterialID()
{
    int programID = (int) _glProgramState->getGLProgram()->getProgram();
    int intArray[4] = { programID, (int)_textureID, (int)_blendType.src, (int)_blendType.dst};
    _materialID = XXH32((const void*) intArray, sizeof(intArray), 0);
}

void QuadCommand::useMaterial() const
{
    GL::bindTexture2D(_textureID);

    GL::blendFunc(_blendType.src, _blendType.dst);

    _glProgramState->apply(_mv);
}

NS_END