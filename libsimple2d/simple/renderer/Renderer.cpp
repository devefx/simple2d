#include "renderer/Renderer.h"

#include <algorithm>

#include "renderer/CustomCommand.h"
#include "renderer/GLStateCache.h"

static const int DEFAULT_RENDER_QUEUE = 0;

Renderer::Renderer()
    : _lastMaterialID(0)
    , _numberQuads(0)
    , _glViewAssigned(false)
    , _isRendering(false)
    , _isDepthTestFor2D(false)
{
    _commandGroupStack.push(DEFAULT_RENDER_QUEUE);

    RenderQueue defaultRenderQueue;
    _renderGroups.push_back(defaultRenderQueue);
    _batchQuadCommands.reserve(BATCH_QUADCOMMAND_RESEVER_SIZE);
}

Renderer::~Renderer()
{
    _renderGroups.clear();
    
    glDeleteBuffers(2, _quadbuffersVBO);

#if ENABLE_VAO
    glDeleteVertexArrays(1, &_quadVAO);
    GL::bindVAO(0);
#endif
}

void Renderer::initGLView()
{
    // setup index data for quads
    for( int i=0; i < VBO_SIZE / 4; i++)
    {
        _quadIndices[i*6+0] = (GLushort) (i*4+0);
        _quadIndices[i*6+1] = (GLushort) (i*4+1);
        _quadIndices[i*6+2] = (GLushort) (i*4+2);
        _quadIndices[i*6+3] = (GLushort) (i*4+3);
        _quadIndices[i*6+4] = (GLushort) (i*4+2);
        _quadIndices[i*6+5] = (GLushort) (i*4+1);
    }

    setupBuffer();

    _glViewAssigned = true;
}

void Renderer::setupBuffer()
{
    // generate vbo and vao for quadCommand
#if ENABLE_VAO
    glGenVertexArrays(1, &_quadVAO);
    GL::bindVAO(_quadVAO);
#endif

    glGenBuffers(2, &_quadbuffersVBO[0]);

    glBindBuffer(GL_ARRAY_BUFFER, _quadbuffersVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(_quadVerts[0]) * VBO_SIZE, _quadVerts, GL_DYNAMIC_DRAW);

    // vertices
    glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_POSITION);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(V3F_C4B_T2F), (GLvoid*) offsetof(V3F_C4B_T2F, vertices));

    // colors
    glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_COLOR);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(V3F_C4B_T2F), (GLvoid*) offsetof(V3F_C4B_T2F, colors));

    // tex coords
    glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_TEX_COORD);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(V3F_C4B_T2F), (GLvoid*) offsetof(V3F_C4B_T2F, texCoords));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _quadbuffersVBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_quadIndices[0]) * INDEX_VBO_SIZE, _quadIndices, GL_STATIC_DRAW);

    // Must unbind the VAO before changing the element buffer.
#if ENABLE_VAO
    GL::bindVAO(0);
#endif
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer::addCommand(RenderCommand* command)
{
    int renderQueue = _commandGroupStack.top();
    addCommand(command, renderQueue);
}

void Renderer::addCommand(RenderCommand* command, int renderQueue)
{
    _renderGroups[renderQueue].push_back(command);
}

void Renderer::pushGroup(int renderQueueID)
{
    // !_isRendering
    _commandGroupStack.push(renderQueueID);
}

void Renderer::popGroup()
{
    // !_isRendering
    _commandGroupStack.pop();
}

int Renderer::createRenderQueue()
{
    RenderQueue newRenderQueue;
    _renderGroups.push_back(newRenderQueue);
    return (int)_renderGroups.size() - 1;
}

void Renderer::processRenderCommand(RenderCommand* command)
{
    auto commandType = command->getType();
    if (commandType == RenderCommand::Type::QUAD_COMMAND)
    {
        auto cmd = static_cast<QuadCommand*>(command);

        if (cmd->isSkipBatching() || ((_numberQuads + cmd->getQuadCount()) * 4) > VBO_SIZE)
        {
            drawBatchedQuads();
        }

        _batchQuadCommands.push_back(cmd);

        fillQuads(cmd);

        if (cmd->isSkipBatching())
        {
            drawBatchedQuads();
        }
    }
    else if (commandType == RenderCommand::Type::CUSTOM_COMMAND)
    {
        flush();
        auto cmd = static_cast<CustomCommand*>(command);
        cmd->execute();
    }

}

void Renderer::visitRenderQueue(RenderQueue& queue)
{
    queue.saveRenderState();

    const auto& zNegQueue = queue.getSubQueue(RenderQueue::GLOBALZ_NEG);
    if (zNegQueue.size() > 0)
    {
        if(_isDepthTestFor2D)
        {
            glEnable(GL_DEPTH_TEST);
            glDepthMask(true);
        }
        else
        {
            glDisable(GL_DEPTH_TEST);
            glDepthMask(false);
        }
        for (auto it = zNegQueue.cbegin(); it != zNegQueue.cend(); it++)
        {
            processRenderCommand(*it);
        }
        flush();
    }

    const auto& zZeroQueue = queue.getSubQueue(RenderQueue::GLOBALZ_ZERO);
    if (zZeroQueue.size() > 0)
    {
        if(_isDepthTestFor2D)
        {
            glEnable(GL_DEPTH_TEST);
            glDepthMask(true);
        }
        else
        {
            glDisable(GL_DEPTH_TEST);
            glDepthMask(false);
        }
        for (auto it = zZeroQueue.cbegin(); it != zZeroQueue.cend(); it++)
        {
            processRenderCommand(*it);
        }
        flush();
    }

    const auto& zPosQueue = queue.getSubQueue(RenderQueue::GLOBALZ_POS);
    if (zPosQueue.size() > 0)
    {
        for (auto it = zPosQueue.cbegin(); it != zPosQueue.cend(); it++)
        {
            processRenderCommand(*it);
        }
        flush();
    }

    queue.restoreRenderState();
}

void Renderer::render()
{
    _isRendering = true;

    if (_glViewAssigned)
    {
        for (auto &renderQueue : _renderGroups)
        {
            renderQueue.sort();
        }
        visitRenderQueue(_renderGroups[0]);
    }
    clean();
    _isRendering = false;
}

void Renderer::clean()
{
    for (size_t i = 0; i < _renderGroups.size(); i++)
    {
        _renderGroups[i].clear();
    }
    // clear batch commands
    _batchQuadCommands.clear();
    _numberQuads = 0;
    _lastMaterialID = 0;
}

void Renderer::clear()
{
    glDepthMask(true);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDepthMask(false);
}

void Renderer::setClearColor(const Color4F& clearColor)
{
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
}

void Renderer::setDepthTest(bool enable)
{
    if (enable)
    {
        glClearDepth(1.0f);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
    }
    else
    {
        glDisable(GL_DEPTH_TEST);
    }
    _isDepthTestFor2D = enable;
}

void Renderer::fillQuads(const QuadCommand *cmd)
{
    const Mat4& modelView = cmd->getModelView();
    const V3F_C4B_T2F* quads = (V3F_C4B_T2F*) cmd->getQuads();
    for (size_t i = 0; i < cmd->getQuadCount() * 4; i++)
    {
        _quadVerts[i + _numberQuads *4] = quads[i];
        modelView.transformPoint(quads[i].vertices, &(_quadVerts[i + _numberQuads * 4].vertices));
    }
    _numberQuads += cmd->getQuadCount();
}

void Renderer::drawBatchedQuads()
{
    int indexToDraw = 0;
    int startIndex = 0;

    if(_numberQuads <= 0 || _batchQuadCommands.empty())
    {
        return;
    }
    
#if ENABLE_VAO
    GL::bindVAO(_quadVAO);
#endif
    glBindBuffer(GL_ARRAY_BUFFER, _quadbuffersVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(_quadVerts[0]) * _numberQuads * 4 , _quadVerts, GL_DYNAMIC_DRAW);
#if ENABLE_VAO
    void *buf = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    memcpy(buf, _quadVerts, sizeof(_quadVerts[0]) * _numberQuads * 4);
    glUnmapBuffer(GL_ARRAY_BUFFER);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
#else
    #define kQuadSize sizeof(_quadVerts[0])
    GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);
    // vertices
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, kQuadSize, (GLvoid*) offsetof(V3F_C4B_T2F, vertices));
    // colors
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (GLvoid*) offsetof(V3F_C4B_T2F, colors));
    // tex coords
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, kQuadSize, (GLvoid*) offsetof(V3F_C4B_T2F, texCoords));
#endif

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _quadbuffersVBO[1]);

    // äÖÈ¾Í¼ÐÎ
    for (const auto& cmd : _batchQuadCommands)
    {
        auto newMaterialID = cmd->getMaterialID();
        if (_lastMaterialID != newMaterialID || cmd->isSkipBatching())
        {
            if (indexToDraw > 0)
            {
                glDrawElements(GL_TRIANGLES, (GLsizei) indexToDraw, GL_UNSIGNED_SHORT, (GLvoid*) (startIndex * sizeof(_quadIndices[0])));
                _drawnBatches ++;
                _drawnVertices += indexToDraw;

                startIndex += indexToDraw;
                indexToDraw = 0;
            }
            cmd->useMaterial();
            _lastMaterialID = newMaterialID;
        }
        indexToDraw += cmd->getQuadCount() * 6;
    }

    if (indexToDraw > 0)
    {
        glDrawElements(GL_TRIANGLES, (GLsizei) indexToDraw, GL_UNSIGNED_SHORT, (GLvoid*) (startIndex * sizeof(_quadIndices[0])));
        _drawnBatches ++;
        _drawnVertices += indexToDraw;
    }

#if ENABLE_VAO
    GL::bindVAO(0);
#else
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#endif

    _batchQuadCommands.clear();
    _numberQuads = 0;
}

void Renderer::flush()
{
    if (_numberQuads > 0)
    {
        drawBatchedQuads();
        _lastMaterialID = 0;
    }
}