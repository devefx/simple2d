#include "main.h"

#include <stdio.h>

#include "GL/glew.h"
#include "glfw3.h"
#include "Bitmap.h"

#include "renderer/Renderer.h"
#include "renderer/CustomCommand.h"
#include "renderer/GLStateCache.h"

#if _MSC_VER > 1800
#pragma comment(lib,"glfw3-2015.lib")
#else
#pragma comment(lib,"glfw3.lib")
#endif


USING_NS;


#define STRINGIFY(A)  #A

// v
const GLchar* vShaderByteArray = STRINGIFY(

attribute vec4 a_position;
attribute vec2 a_texCoord;
attribute vec4 a_color;

\n#ifdef GL_ES\n
    varying lowp vec4 v_fragmentColor;
varying mediump vec2 v_texCoord;
\n#else\n
    varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
\n#endif\n

    void main()
{
    gl_Position = PMatrix * a_position;
    v_fragmentColor = a_color;
    v_texCoord = a_texCoord;
}

);
// f
const GLchar* fShaderByteArray = STRINGIFY(

\n#ifdef GL_ES\n
precision lowp float;
\n#endif\n

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

void main()
{
    gl_FragColor = v_fragmentColor * texture2D(Texture0, v_texCoord);
}

);


static bool glew_dynamic_binding()
{
    const char *gl_extensions = (const char*)glGetString(GL_EXTENSIONS);

    // If the current opengl driver doesn't have framebuffers methods, check if an extension exists
    if (glGenFramebuffers == nullptr)
    {
        if (strstr(gl_extensions, "ARB_framebuffer_object"))
        {
            glIsRenderbuffer = (PFNGLISRENDERBUFFERPROC) wglGetProcAddress("glIsRenderbuffer");
            glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC) wglGetProcAddress("glBindRenderbuffer");
            glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC) wglGetProcAddress("glDeleteRenderbuffers");
            glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC) wglGetProcAddress("glGenRenderbuffers");
            glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC) wglGetProcAddress("glRenderbufferStorage");
            glGetRenderbufferParameteriv = (PFNGLGETRENDERBUFFERPARAMETERIVPROC) wglGetProcAddress("glGetRenderbufferParameteriv");
            glIsFramebuffer = (PFNGLISFRAMEBUFFERPROC) wglGetProcAddress("glIsFramebuffer");
            glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC) wglGetProcAddress("glBindFramebuffer");
            glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC) wglGetProcAddress("glDeleteFramebuffers");
            glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC) wglGetProcAddress("glGenFramebuffers");
            glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC) wglGetProcAddress("glCheckFramebufferStatus");
            glFramebufferTexture1D = (PFNGLFRAMEBUFFERTEXTURE1DPROC) wglGetProcAddress("glFramebufferTexture1D");
            glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC) wglGetProcAddress("glFramebufferTexture2D");
            glFramebufferTexture3D = (PFNGLFRAMEBUFFERTEXTURE3DPROC) wglGetProcAddress("glFramebufferTexture3D");
            glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC) wglGetProcAddress("glFramebufferRenderbuffer");
            glGetFramebufferAttachmentParameteriv = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC) wglGetProcAddress("glGetFramebufferAttachmentParameteriv");
            glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC) wglGetProcAddress("glGenerateMipmap");
        }
        else
            if (strstr(gl_extensions, "EXT_framebuffer_object"))
            {
                glIsRenderbuffer = (PFNGLISRENDERBUFFERPROC) wglGetProcAddress("glIsRenderbufferEXT");
                glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC) wglGetProcAddress("glBindRenderbufferEXT");
                glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC) wglGetProcAddress("glDeleteRenderbuffersEXT");
                glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC) wglGetProcAddress("glGenRenderbuffersEXT");
                glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC) wglGetProcAddress("glRenderbufferStorageEXT");
                glGetRenderbufferParameteriv = (PFNGLGETRENDERBUFFERPARAMETERIVPROC) wglGetProcAddress("glGetRenderbufferParameterivEXT");
                glIsFramebuffer = (PFNGLISFRAMEBUFFERPROC) wglGetProcAddress("glIsFramebufferEXT");
                glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC) wglGetProcAddress("glBindFramebufferEXT");
                glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC) wglGetProcAddress("glDeleteFramebuffersEXT");
                glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC) wglGetProcAddress("glGenFramebuffersEXT");
                glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC) wglGetProcAddress("glCheckFramebufferStatusEXT");
                glFramebufferTexture1D = (PFNGLFRAMEBUFFERTEXTURE1DPROC) wglGetProcAddress("glFramebufferTexture1DEXT");
                glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC) wglGetProcAddress("glFramebufferTexture2DEXT");
                glFramebufferTexture3D = (PFNGLFRAMEBUFFERTEXTURE3DPROC) wglGetProcAddress("glFramebufferTexture3DEXT");
                glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC) wglGetProcAddress("glFramebufferRenderbufferEXT");
                glGetFramebufferAttachmentParameteriv = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC) wglGetProcAddress("glGetFramebufferAttachmentParameterivEXT");
                glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC) wglGetProcAddress("glGenerateMipmapEXT");
            }
            else
            {
                return false;
            }
    }
    return true;
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* _mainWindow = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!_mainWindow)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(_mainWindow);

    /* glew init */
    if (glewInit() != GLEW_OK)
    {
        glfwTerminate();
        return -1;
    }
    /* glew bind */
    glew_dynamic_binding();


    Renderer* render = new Renderer();

    render->initGLView();


    CustomCommand* command = new CustomCommand();
    command->init(0, []() {
        GL::useProgram(0);
        
        glBegin(GL_TRIANGLES);

        glColor3f(1.0, 0.0, 0.0);    // Red
        glVertex3f(0.0, 1.0, 0.0);

        glColor3f(0.0, 1.0, 0.0);    // Green
        glVertex3f(-1.0, -1.0, 0.0);

        glColor3f(0.0, 0.0, 1.0);    // Blue
        glVertex3f(1.0, -1.0, 0.0);

        glEnd();

    });
    
    float x1 = 0.0f;
    float y1 = 0.0f;
    float x2 = 300.0f;
    float y2 = 300.0f;

    float globalZOrder = 0;
    GLuint textureID = 0;


    Bitmap bmp("F:\\1.bmp");
    // ��������
    unsigned char *data = (unsigned char*)bmp.getBitmap();

    

    /*
    size_t size = 0;
    FILE* file = fopen("F:\\1.bmp", "r");
    fseek(file, 0, SEEK_END);
    size = ftell(file) - 54;
    fseek(file, 54, SEEK_SET);

    data = (unsigned char*)malloc(sizeof(unsigned char) * size);
    fread(data, sizeof(unsigned char), size, file);
    fclose(file);
    */
    // ���ö���
    glPixelStorei(GL_UNPACK_ALIGNMENT, 8);
    glGenTextures(1, &textureID);
    //��Ϊ������˲�������
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);


    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
        return -1;
    }

    // ��ɫ��
    GLProgram* glprogram = GLProgram::createWithByteArrays(vShaderByteArray, fShaderByteArray);
    GLProgramState* glProgramState = GLProgramState::create(glprogram);
    
    // ���嶥������
    V3F_C4B_T2F_Quad quad;
    quad.bl.vertices = Vec3(x1, y1, 0);
    quad.br.vertices = Vec3(x2, y1, 0);
    quad.tl.vertices = Vec3(x1, y2, 0);
    quad.tr.vertices = Vec3(x2, y2, 0);

    quad.bl.texCoords = Tex2F(0, 0);
    quad.br.texCoords = Tex2F(1, 0);
    quad.tl.texCoords = Tex2F(0, 1);
    quad.tr.texCoords = Tex2F(1, 1);

    quad.bl.colors = Color4B::RED;
    quad.br.colors = Color4B::RED;
    quad.tl.colors = Color4B::RED;
    quad.tr.colors = Color4B::RED;
    // ����λ��
    Mat4 transform;
    transform.m[12] = 100.0;
    transform.m[13] = 100.0;

    QuadCommand* quadCommand = new QuadCommand();
    quadCommand->init(globalZOrder, textureID, glProgramState, BlendFunc::ALPHA_PREMULTIPLIED, &quad, 1, transform);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(_mainWindow))
    {
        render->addCommand(command);
        render->addCommand(quadCommand);

        render->clear();

        render->render();

        /* Swap front and back buffers */
        glfwSwapBuffers(_mainWindow);

        /* Poll for and process events */
        glfwPollEvents();

		Sleep(1);
    }

    glfwTerminate();
    return 0;
}