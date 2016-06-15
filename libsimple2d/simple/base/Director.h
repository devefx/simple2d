#ifndef __BASE_DIRECTOR_H__
#define __BASE_DIRECTOR_H__

#include "platform/PlatformMacros.h"
#include "platform/GL.h"
#include "platform/GLView.h"
#include "base/Ref.h"
#include "base/MatrixControl.h"

/**
 * @addtogroup base
 * @{
 */
NS_BEGIN

class DLL Director : public Ref
{
public:

    static Director* getInstance();

    Director();

    virtual ~Director();
    virtual bool init();

    //

    /** Gets the FPS value. */
    inline double getAnimationInterval() { return _animationInterval; }
    /** Sets the FPS value. FPS = 1/internal. */
    virtual void setAnimationInterval(double interval) = 0;

    /** Whether or not to display the FPS on the bottom-left corner. */
    inline bool isDisplayStats() { return _displayStats; }
    /** Display the FPS on the bottom-left corner. */
    inline void setDisplayStats(bool displayStats) { _displayStats = displayStats; }

    //

    inline GLView* getOpenGLView() { return _openGLView; }

    void setOpenGLView(GLView *openGLView);

    //

    void end();

    void pause();

    void resume();

    void restart();

    virtual void stopAnimation() = 0;

    virtual void startAnimation() = 0;

    //

    virtual void mainLoop() = 0;


protected:
    void reset();

    void purgeDirector();
    bool _purgeDirectorInNextLoop; // this flag will be set to true in end()

    void restartDirector();
    bool _restartDirectorInNextLoop; // this flag will be set to true in restart()


    MatrixControl *_matrixControl;


    GLView *_openGLView;


    bool _displayStats;


    double _animationInterval;
    double _oldAnimationInterval;

    bool _isStatusLabelUpdated;

    friend class GLView;
};

//----------------------------------------------------------------------------
//
// DisplayLinkDirector
//
//----------------------------------------------------------------------------
class DisplayLinkDirector : public Director
{
public:
    DisplayLinkDirector() : _invalid(false) {}

    virtual ~DisplayLinkDirector() {}

    // Overrides
    virtual void mainLoop() override;
    virtual void setAnimationInterval(double interval) override;
    virtual void startAnimation() override;
    virtual void stopAnimation() override;

protected:
    bool _invalid;
};

NS_END
/**
 end of base group
 @}
 */

#endif // !__BASE_DIRECTOR_H__
