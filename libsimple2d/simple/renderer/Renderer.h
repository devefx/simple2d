#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <vector>
#include <stack>

// temp
#include "base/Types.h"
#include "renderer/GLProgram.h"
#include "renderer/QuadCommand.h"
#include "renderer/RenderQueue.h"


class DLL Renderer
{
public:
    /**The max number of vertices in a vertex buffer object.*/
    static const int VBO_SIZE = 65536;
    /**The max numer of indices in a index buffer.*/
    static const int INDEX_VBO_SIZE = VBO_SIZE * 6 / 4;
    /**The rendercommands which can be batched will be saved into a list, this is the reversed size of this list.*/
    static const int BATCH_QUADCOMMAND_RESEVER_SIZE = 64;

    Renderer();

    ~Renderer();

    void initGLView();

    void addCommand(RenderCommand* command);

    void addCommand(RenderCommand* command, int renderQueue);

    void pushGroup(int renderQueueID);

    void popGroup();

    int createRenderQueue();

    void render();

    void clean();

    void clear();

    void setClearColor(const Color4F& clearColor);

    size_t getDrawnBatches() const { return _drawnBatches; }

    size_t getDrawnVertices() const { return _drawnVertices; }

    void clearDrawStats() { _drawnBatches = _drawnVertices = 0; }

    void setDepthTest(bool enable);

protected:

    void setupBuffer();
    void drawBatchedQuads();

    void flush();

    void fillQuads(const QuadCommand* cmd);

    void processRenderCommand(RenderCommand* command);
    void visitRenderQueue(RenderQueue& queue);

    std::stack<int> _commandGroupStack;
    std::vector<RenderQueue> _renderGroups;

    // for QuadCommand
    V3F_C4B_T2F _quadVerts[VBO_SIZE];
    GLushort _quadIndices[INDEX_VBO_SIZE];
    GLuint _quadVAO;
    GLuint _quadbuffersVBO[2]; //0: vertex  1: indices

    std::vector<QuadCommand*> _batchQuadCommands;
    int _numberQuads;

    uint32_t _lastMaterialID;

    bool _glViewAssigned;

    bool _isRendering;

    bool _isDepthTestFor2D;

    // state
    size_t _drawnBatches;
    size_t _drawnVertices;

};

#endif // !__Renderer_h__
