#ifndef __QUAD_COMMAND_H__
#define __QUAD_COMMAND_H__

#include "renderer/RenderCommand.h"
#include "renderer/GLProgramState.h"

// temp
#include "platform/GL.h"
#include <stddef.h>

#include "base/Types.h"
#include "math/Mat4.h"


class DLL QuadCommand : public RenderCommand
{
public:
    QuadCommand();

    void init(float globalOrder, GLuint textureID, GLProgramState* shader, const BlendFunc& blendType, V3F_C4B_T2F_Quad* quads,
        size_t quadCount, const Mat4& mv);

    void useMaterial() const;

    inline uint32_t getMaterialID() const { return _materialID; }

    inline GLuint getTextureID() const { return _textureID; }

    inline V3F_C4B_T2F_Quad* getQuads() const { return _quads; }

    inline size_t getQuadCount() const { return _quadsCount; }

    inline GLProgramState* getGLProgramState() const { return _glProgramState; }

    inline BlendFunc getBlendType() const { return _blendType; }

    inline const Mat4& getModelView() const { return _mv; }

    inline virtual RenderCommand::Type getType() const override
    {
        return RenderCommand::Type::QUAD_COMMAND;
    }

protected:
    void generateMaterialID();

    uint32_t _materialID;   // 材料ID

    GLuint _textureID;      // 纹理ID

    GLProgramState* _glProgramState;    // GL程序状态

    BlendFunc _blendType;   // 绑定方法

    V3F_C4B_T2F_Quad* _quads;   // 顶点数据

    size_t _quadsCount;        // 顶点数量

    Mat4 _mv;               // modelView
};


#endif // !__QUAD_COMMAND_H__
