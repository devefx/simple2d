#ifndef __RENDERER_H__
#define __RENDERER_H__


#define DLL __declspec(dllexport)
#include "base/Types.h"
#include "renderer/GLProgram.h"

class DLL Renderer
{
public:
    /**The max number of vertices in a vertex buffer object.*/
    static const int VBO_SIZE = 65536;
    /**The max numer of indices in a index buffer.*/
    static const int INDEX_VBO_SIZE = VBO_SIZE * 6 / 4;


    Renderer();

    ~Renderer();

    void initGLView();

    void render();

    void clean();

    void clear();

protected:

    void setupBuffer();



    // for QuadCommand
    V3F_C4B_T2F _quadVerts[VBO_SIZE];
    GLushort _quadIndices[INDEX_VBO_SIZE];
    GLuint _quadVAO;
    GLuint _quadbuffersVBO[2]; //0: vertex  1: indices
    int _numberQuads;

    bool _glViewAssigned;

    // state
    long _drawnBatches;
    long _drawnVertices;

    bool _isRendering;
};

#endif // !__Renderer_h__
