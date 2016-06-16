#ifndef __GLVIEW_H__
#define __GLVIEW_H__

#include "math/Geometry.h"
#include "base/Ref.h"
#include "platform/GL.h"
#include <string>

#if TARGET_PLATFORM == PLATFORM_WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#elif TARGET_PLATFORM == PLATFORM_MAC
    typedef void* id;
#endif

/** There are some Resolution Policy for Adapt to the screen. */
enum class ResolutionPolicy
{
    /** The entire application is visible in the specified area without trying to preserve the original aspect ratio.
     * Distortion can occur, and the application may appear stretched or compressed.
     */
    EXACT_FIT,
    /** The entire application fills the specified area, without distortion but possibly with some cropping,
     * while maintaining the original aspect ratio of the application.
     */
    NO_BORDER,
    /** The entire application is visible in the specified area without distortion while maintaining the original
     * aspect ratio of the application. Borders can appear on two sides of the application.
     */
    SHOW_ALL,
    /** The application takes the height of the design resolution size and modifies the width of the internal
     * canvas so that it fits the aspect ratio of the device.
     * No distortion will occur however you must make sure your application works on different
     * aspect ratios.
     */
    FIXED_HEIGHT,
    /** The application takes the width of the design resolution size and modifies the height of the internal
     * canvas so that it fits the aspect ratio of the device.
     * No distortion will occur however you must make sure your application works on different
     * aspect ratios.
     */
    FIXED_WIDTH,

    UNKNOWN,
};

/** @struct GLContextAttrs 
 *
 * There are six opengl Context Attrs. 
 */
struct GLContextAttrs
{
    int redBits;
    int greenBits;
    int blueBits;
    int alphaBits;
    int depthBits;
    int stencilBits;
};

/**
 * @addtogroup platform
 * @{
 */
NS_BEGIN

class DLL GLView : public Ref
{
public:
    GLView();

    ~GLView();

    virtual void end() = 0;

    virtual bool isOpenGLReady() = 0;

    virtual void swapBuffers() = 0;

    virtual void setIMEKeyboardState(bool open) = 0;

    virtual bool windowShouldClose() { return false; };

    static void setGLContextAttrs(GLContextAttrs& glContextAttrs);

    static GLContextAttrs getGLContextAttrs();

    static GLContextAttrs _glContextAttrs;

    virtual void pollEvents();

    virtual const Size& getFrameSize() const;

    virtual void setFrameSize(float width, float height);

    virtual void setFrameZoomFactor(float zoomFactor) {}

    virtual float getFrameZoomFactor() const { return 1.0; }

    virtual void setCursorVisible(bool isVisible) {}

    //

#if (TARGET_PLATFORM == PLATFORM_IOS)
    virtual void* getEAGLView() const { return nullptr; }
#endif // TARGET_PLATFORM == PLATFORM_IOS

    virtual Size getVisibleSize() const;

    virtual Vec2 getVisibleOrigin() const;

    virtual Rect getVisibleRect() const;

    virtual void setDesignResolutionSize(float width, float height, ResolutionPolicy resolutionPolicy);

    virtual const Size&  getDesignResolutionSize() const;

    virtual void setViewPortInPoints(float x , float y , float w , float h);

    virtual void setScissorInPoints(float x , float y , float w , float h);

    virtual bool isScissorEnabled();

    virtual Rect getScissorRect() const;

    virtual void setViewName(const std::string& viewname);

    const std::string& getViewName() const;

    virtual void handleTouchesBegin(int num, intptr_t ids[], float xs[], float ys[]);

    virtual void handleTouchesMove(int num, intptr_t ids[], float xs[], float ys[]);

    virtual void handleTouchesEnd(int num, intptr_t ids[], float xs[], float ys[]);

    virtual void handleTouchesCancel(int num, intptr_t ids[], float xs[], float ys[]);

    const Rect& getViewPortRect() const;

    float getScaleX() const;

    float getScaleY() const;

    ResolutionPolicy getResolutionPolicy() const { return _resolutionPolicy; }

#if TARGET_PLATFORM == PLATFORM_WIN32
    virtual HWND getWin32Window() = 0;
#elif TARGET_PLATFORM == PLATFORM_MAC
    virtual id getCocoaWindow() = 0;
#endif

protected:
    void updateDesignResolutionSize();

    //void handleTouchesOfEndOrCancel(EventTouch::EventCode eventCode, int num, intptr_t ids[], float xs[], float ys[]);

    Size _screenSize;

    Size _designResolutionSize;

    Rect _viewPortRect;

    std::string _viewName;

    float _scaleX;
    float _scaleY;
    ResolutionPolicy _resolutionPolicy;
};

NS_END
/**
 end of platform group
 @}
 */

#endif // !__GLVIEW_H__
