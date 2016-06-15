#ifndef __EVENT_H__
#define __EVENT_H__

#include "base/Ref.h"
#include "platform/PlatformMacros.h"

/**
 * @addtogroup event
 * @{
 */
NS_BEGIN

class DLL Event : public Ref
{
public:
    /** Type Event type.*/
    enum class Type
    {
        TOUCH,
        KEYBOARD,
        ACCELERATION,
        MOUSE,
        FOCUS,
        GAME_CONTROLLER,
        CUSTOM
    };
protected:
    Event(Type type);
public:

    virtual ~Event();

    inline Type getType() const { return _type; };

    inline void stopPropagation() { _isStopped = true; };

    inline bool isStopped() const { return _isStopped; };



protected:

    Type _type;

    bool _isStopped;


};

NS_END
/**
 end of event group
 @}
 */

#endif // !__EVENT_H__
