#ifndef __GLVIEW_H__
#define __GLVIEW_H__

#include "base/Ref.h"
#include <string>

#if TARGET_PLATFORM == PLATFORM_WIN32
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

    virtual bool windowShouldClose() { return false; };

    virtual void pollEvents();

    virtual void setCursorVisible(bool isVisible) {}

    virtual void setViewName(const std::string& viewname);

    const std::string& getViewName() const;



    float getScaleX() const;

    float getScaleY() const;

    ResolutionPolicy getResolutionPolicy() const { return _resolutionPolicy; }

#if TARGET_PLATFORM == PLATFORM_WIN32
    virtual HWND getWin32Window() = 0;
#elif TARGET_PLATFORM == PLATFORM_MAC
    virtual id getCocoaWindow() = 0;
#endif

protected:
    std::string _viewName;

    float _scaleX;
    float _scaleY;
    ResolutionPolicy _resolutionPolicy;
};

NS_END
// end of platform group
/// @}

#endif // !__GLVIEW_H__
