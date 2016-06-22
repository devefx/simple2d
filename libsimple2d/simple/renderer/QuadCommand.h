#ifndef __QUAD_COMMAND_H__
#define __QUAD_COMMAND_H__

#include "renderer/RenderCommand.h"

// temp
#define DLL __declspec(dllexport)
#include "GL/glew.h"
#include "base/Types.h"
#include "math/Mat4.h"

class DLL QuadCommand : public RenderCommand
{
public:
    QuadCommand();

    void useMaterial() const;

    inline uint32_t getMaterialID() const { return _materialID; }

    inline GLuint getTextureID() const { return _textureID; }

    inline V3F_C4B_T2F_Quad* getQuads() const { return _quads; }

    inline size_t getQuadCount() const { return _quadsCount; }

    //

    inline BlendFunc getBlendType() const { return _blendType; }

    inline const Mat4& getModelView() const { return _mv; }


    virtual RenderCommand::Type getType() const override;

protected:
    void generateMaterialID();

    uint32_t _materialID;

    GLuint _textureID;

    //

    BlendFunc _blendType;

    V3F_C4B_T2F_Quad* _quads;

    int _quadsCount;

    Mat4 _mv;
};


#endif // !__QUAD_COMMAND_H__
