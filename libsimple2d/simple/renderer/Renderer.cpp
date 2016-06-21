#include "renderer/Renderer.h"

#include "GL/glew.h"

#include <algorithm>

Renderer::Renderer()
{

}

Renderer::~Renderer()
{

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

void Renderer::render()
{
    _isRendering = true;

    if (_glViewAssigned)
    {
        // draw
        glBegin(GL_TRIANGLES);

        glColor3f(1.0, 0.0, 0.0);    // Red
        glVertex3f(0.0, 1.0, 0.0);

        glColor3f(0.0, 1.0, 0.0);    // Green
        glVertex3f(-1.0, -1.0, 0.0);

        glColor3f(0.0, 0.0, 1.0);    // Blue
        glVertex3f(1.0, -1.0, 0.0);

        glEnd();
    }
    clean();
    _isRendering = false;
}

void Renderer::clean()
{

}

void Renderer::clear()
{
    glDepthMask(true);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDepthMask(false);
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
