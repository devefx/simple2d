#include "platform/desktop/GLViewImpl-desktop.h"

#include <unordered_map>

NS_BEGIN

class GLFWEventHandler
{
public:
    static void onGLFWError(int errorID, const char* errorDesc)
    {
        if (_view)
            _view->onGLFWError(errorID, errorDesc);
    }

    static void onGLFWMouseCallBack(GLFWwindow* window, int button, int action, int modify)
    {
        if (_view)
            _view->onGLFWMouseCallBack(window, button, action, modify);
    }

    static void onGLFWMouseMoveCallBack(GLFWwindow* window, double x, double y)
    {
        if (_view)
            _view->onGLFWMouseMoveCallBack(window, x, y);
    }

    static void onGLFWMouseScrollCallback(GLFWwindow* window, double x, double y)
    {
        if (_view)
            _view->onGLFWMouseScrollCallback(window, x, y);
    }

    static void onGLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (_view)
            _view->onGLFWKeyCallback(window, key, scancode, action, mods);
    }

    static void onGLFWCharCallback(GLFWwindow* window, unsigned int character)
    {
        if (_view)
            _view->onGLFWCharCallback(window, character);
    }

    static void onGLFWWindowPosCallback(GLFWwindow* windows, int x, int y)
    {
        if (_view)
            _view->onGLFWWindowPosCallback(windows, x, y);
    }

    static void onGLFWframebuffersize(GLFWwindow* window, int w, int h)
    {
        if (_view)
            _view->onGLFWframebuffersize(window, w, h);
    }

    static void onGLFWWindowSizeFunCallback(GLFWwindow *window, int width, int height)
    {
        if (_view)
            _view->onGLFWWindowSizeFunCallback(window, width, height);
    }

    static void setGLViewImpl(GLViewImpl* view)
    {
        _view = view;
    }

    static void onGLFWWindowIconifyCallback(GLFWwindow* window, int iconified)
    {
        if (_view)
        {
            _view->onGLFWWindowIconifyCallback(window, iconified);
        }
    }
private:
    static GLViewImpl* _view;
};

GLViewImpl* GLFWEventHandler::_view = nullptr;

//--------------------------------------------------------------------
//
// GLViewImpl
//
//--------------------------------------------------------------------

GLViewImpl::GLViewImpl()
: _captured(false)
, _supportTouch(false)
, _isInRetinaMonitor(false)
, _isRetinaEnabled(false)
, _retinaFactor(1)
, _frameZoomFactor(1.0f)
, _mainWindow(nullptr)
, _monitor(nullptr)
, _mouseX(0.0f)
, _mouseY(0.0f)
{
    _viewName = "simple2d";

    GLFWEventHandler::setGLViewImpl(this);
    glfwSetErrorCallback(GLFWEventHandler::onGLFWError);
    glfwInit();
}

GLViewImpl::~GLViewImpl()
{
    GLFWEventHandler::setGLViewImpl(nullptr);
    glfwTerminate();
}

GLViewImpl* GLViewImpl::create(const std::string& viewName)
{
    auto ret = new (std::nothrow) GLViewImpl;
    if (ret && ret->initWithRect(viewName, 800, 600, 1))
    {
        ret->autorelease();
        return ret;
    }
    return nullptr;
}

GLViewImpl* GLViewImpl::createWithRect(const std::string& viewName, int width, int height, float frameZoomFactor)
{
    auto ret = new (std::nothrow) GLViewImpl;
    if (ret && ret->initWithRect(viewName, width, height, 1))
    {
        ret->autorelease();
        return ret;
    }
    return nullptr;
}

GLViewImpl* GLViewImpl::createWithFullScreen(const std::string& viewName)
{
    auto ret = new (std::nothrow) GLViewImpl;
    if (ret && ret->initWithFullScreen(viewName))
    {
        ret->autorelease();
        return ret;
    }
    return nullptr;
}

GLViewImpl* GLViewImpl::createWithFullScreen(const std::string& viewName, const GLFWvidmode &videoMode, GLFWmonitor *monitor)
{
    auto ret = new (std::nothrow) GLViewImpl();
    if(ret && ret->initWithFullscreen(viewName, videoMode, monitor)) {
        ret->autorelease();
        return ret;
    }
    return nullptr;
}

bool GLViewImpl::initWithRect(const std::string& viewName, int width, int height, float frameZoomFactor)
{
    setViewName(viewName);

    _frameZoomFactor = frameZoomFactor;

    glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);

    _mainWindow = glfwCreateWindow(width * _frameZoomFactor,
                                   height * _frameZoomFactor,
                                   _viewName.c_str(),
                                   _monitor,
                                   nullptr);

    glfwMakeContextCurrent(_mainWindow);

    glfwSetMouseButtonCallback(_mainWindow, GLFWEventHandler::onGLFWMouseCallBack);
    glfwSetCursorPosCallback(_mainWindow, GLFWEventHandler::onGLFWMouseMoveCallBack);
    glfwSetScrollCallback(_mainWindow, GLFWEventHandler::onGLFWMouseScrollCallback);
    glfwSetCharCallback(_mainWindow, GLFWEventHandler::onGLFWCharCallback);
    glfwSetKeyCallback(_mainWindow, GLFWEventHandler::onGLFWKeyCallback);
    glfwSetWindowPosCallback(_mainWindow, GLFWEventHandler::onGLFWWindowPosCallback);
    glfwSetFramebufferSizeCallback(_mainWindow, GLFWEventHandler::onGLFWframebuffersize);
    glfwSetWindowSizeCallback(_mainWindow, GLFWEventHandler::onGLFWWindowSizeFunCallback);
    glfwSetWindowIconifyCallback(_mainWindow, GLFWEventHandler::onGLFWWindowIconifyCallback);

    initGlew();

    // Enable point size by default.
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

    return true;
}

bool GLViewImpl::initWithFullScreen(const std::string& viewName)
{
    // Create fullscreen window on primary monitor at its current video mode.
    _monitor = glfwGetPrimaryMonitor();
    if (nullptr == _monitor)
        return false;

    const GLFWvidmode* videoMode = glfwGetVideoMode(_monitor);
    return initWithRect(viewName, videoMode->width, videoMode->height, 1.0f);
}

bool GLViewImpl::initWithFullscreen(const std::string &viewname, const GLFWvidmode &videoMode, GLFWmonitor *monitor)
{
    //Create fullscreen on specified monitor at the specified video mode.
    _monitor = monitor;
    if (nullptr == _monitor)
        return false;

    //These are soft contraints. If the video mode is retrieved at runtime, the resulting window and context should match these exactly. If invalid attribs are passed (eg. from an outdated cache), window creation will NOT fail but the actual window/context may differ.
    glfwWindowHint(GLFW_REFRESH_RATE, videoMode.refreshRate);
    glfwWindowHint(GLFW_RED_BITS, videoMode.redBits);
    glfwWindowHint(GLFW_BLUE_BITS, videoMode.blueBits);
    glfwWindowHint(GLFW_GREEN_BITS, videoMode.greenBits);

    return initWithRect(viewname, videoMode.width, videoMode.height, 1.0f);
}

bool GLViewImpl::isOpenGLReady()
{
    return nullptr != _mainWindow;
}

void GLViewImpl::end()
{
    if(_mainWindow)
    {
        glfwSetWindowShouldClose(_mainWindow,1);
        _mainWindow = nullptr;
    }
    // Release self. Otherwise, GLViewImpl could not be freed.
    release();
}

void GLViewImpl::swapBuffers()
{
    if(_mainWindow)
        glfwSwapBuffers(_mainWindow);
}

bool GLViewImpl::windowShouldClose()
{
    if(_mainWindow)
        return glfwWindowShouldClose(_mainWindow) ? true : false;
    else
        return true;
}

void GLViewImpl::pollEvents()
{
    glfwPollEvents();
}

void GLViewImpl::onGLFWError(int errorID, const char* errorDesc)
{

}

void GLViewImpl::onGLFWMouseCallBack(GLFWwindow* window, int button, int action, int modify)
{

}

void GLViewImpl::onGLFWMouseMoveCallBack(GLFWwindow* window, double x, double y)
{

}

void GLViewImpl::onGLFWMouseScrollCallback(GLFWwindow* window, double x, double y)
{

}

void GLViewImpl::onGLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

}
void GLViewImpl::onGLFWCharCallback(GLFWwindow* window, unsigned int character)
{

}

void GLViewImpl::onGLFWWindowPosCallback(GLFWwindow* windows, int x, int y)
{

}

void GLViewImpl::onGLFWframebuffersize(GLFWwindow* window, int w, int h)
{

}

void GLViewImpl::onGLFWWindowSizeFunCallback(GLFWwindow *window, int width, int height)
{

}

void GLViewImpl::onGLFWWindowIconifyCallback(GLFWwindow* window, int iconified)
{

}


#if (TARGET_PLATFORM == PLATFORM_WIN32)
static bool glew_dynamic_binding()
{
    const char *gl_extensions = (const char*)glGetString(GL_EXTENSIONS);

    // If the current opengl driver doesn't have framebuffers methods, check if an extension exists
    if (glGenFramebuffers == nullptr)
    {
//        log("OpenGL: glGenFramebuffers is nullptr, try to detect an extension");
        if (strstr(gl_extensions, "ARB_framebuffer_object"))
        {
//            log("OpenGL: ARB_framebuffer_object is supported");

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
//                log("OpenGL: EXT_framebuffer_object is supported");
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
//                log("OpenGL: No framebuffers extension is supported");
//                log("OpenGL: Any call to Fbo will crash!");
                return false;
            }
    }
    return true;
}
#endif

// helper
bool GLViewImpl::initGlew()
{
#if (TARGET_PLATFORM != PLATFORM_MAC)
    GLenum GlewInitResult = glewInit();
    if (GLEW_OK != GlewInitResult)
    {
//        MessageBox((char *)glewGetErrorString(GlewInitResult), "OpenGL error");
        return false;
    }

    if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader)
    {
//        log("Ready for GLSL");
    }
    else
    {
//        log("Not totally ready :(");
    }

    if (glewIsSupported("GL_VERSION_2_0"))
    {
//        log("Ready for OpenGL 2.0");
    }
    else
    {
//        log("OpenGL 2.0 not supported");
    }

#if (TARGET_PLATFORM == PLATFORM_WIN32)
    if(glew_dynamic_binding() == false)
    {
//        MessageBox("No OpenGL framebuffer support. Please upgrade the driver of your video card.", "OpenGL error");
        return false;
    }
#endif

#endif // (TARGET_PLATFORM != PLATFORM_MAC)

    return true;
}

NS_END