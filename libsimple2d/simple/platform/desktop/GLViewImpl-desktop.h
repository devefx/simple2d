#ifndef __GLVIEW_DESKTOP_H__
#define __GLVIEW_DESKTOP_H__

#include "platform/GL.h"

#include "platform/GLView.h"
#include "glfw3.h"

#if TARGET_PLATFORM == PLATFORM_WIN32
    #ifndef GLFW_EXPOSE_NATIVE_WIN32
        #define GLFW_EXPOSE_NATIVE_WIN32
    #endif // !GLFW_EXPOSE_NATIVE_WIN32
    #ifndef GLFW_EXPOSE_NATIVE_WGL
        #define GLFW_EXPOSE_NATIVE_WGL
    #endif // !GLFW_EXPOSE_NATIVE_WGL
    #include "glfw3native.h"
#elif TARGET_PLATFORM == PLATFORM_MAC
    #ifndef GLFW_EXPOSE_NATIVE_NSGL
        #define GLFW_EXPOSE_NATIVE_NSGL
    #endif // !GLFW_EXPOSE_NATIVE_NSGL
    #ifndef GLFW_EXPOSE_NATIVE_COCOA
        #define GLFW_EXPOSE_NATIVE_COCOA
    #endif // !GLFW_EXPOSE_NATIVE_COCOA
    #include "glfw3native.h"
#endif

NS_BEGIN

class DLL GLViewImpl : public GLView
{
public:
    static GLViewImpl* create(const std::string& viewName);
    static GLViewImpl* createWithRect(const std::string& viewName, int width, int height, float frameZoomFactor = 1.0f);
    static GLViewImpl* createWithFullScreen(const std::string& viewName);
    static GLViewImpl* createWithFullScreen(const std::string& viewName, const GLFWvidmode &videoMode, GLFWmonitor *monitor);


    bool windowShouldClose() override;
    void pollEvents() override;
    GLFWwindow* getWindow() const { return _mainWindow; }

    /* override functions */
    virtual bool isOpenGLReady() override;
    virtual void end() override;
    virtual void swapBuffers() override;

#if TARGET_PLATFORM == PLATFORM_WIN32
    HWND getWin32Window() { return glfwGetWin32Window(_mainWindow); }
#elif TARGET_PLATFORM == PLATFORM_MAC
    id getCocoaWindow() { return glfwGetCocoaWindow(_mainWindow); }
#endif

protected:
    GLViewImpl();
    virtual ~GLViewImpl();

    bool initWithRect(const std::string& viewName, int width, int height, float frameZoomFactor);
    bool initWithFullScreen(const std::string& viewName);
    bool initWithFullscreen(const std::string& viewname, const GLFWvidmode &videoMode, GLFWmonitor *monitor);

    bool initGlew();

    // GLFW callbacks
    void onGLFWError(int errorID, const char* errorDesc);
    void onGLFWMouseCallBack(GLFWwindow* window, int button, int action, int modify);
    void onGLFWMouseMoveCallBack(GLFWwindow* window, double x, double y);
    void onGLFWMouseScrollCallback(GLFWwindow* window, double x, double y);
    void onGLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void onGLFWCharCallback(GLFWwindow* window, unsigned int character);
    void onGLFWWindowPosCallback(GLFWwindow* windows, int x, int y);
    void onGLFWframebuffersize(GLFWwindow* window, int w, int h);
    void onGLFWWindowSizeFunCallback(GLFWwindow *window, int width, int height);
    void onGLFWWindowIconifyCallback(GLFWwindow* window, int iconified);

    bool _captured;
    bool _supportTouch;
    bool _isInRetinaMonitor;
    bool _isRetinaEnabled;
    int  _retinaFactor;  // Should be 1 or 2

    float _frameZoomFactor;

    GLFWwindow* _mainWindow;
    GLFWmonitor* _monitor;

    float _mouseX;
    float _mouseY;

    friend class GLFWEventHandler;
private:

};

NS_END

#endif // !__GLVIEW_DESKTOP_H__
