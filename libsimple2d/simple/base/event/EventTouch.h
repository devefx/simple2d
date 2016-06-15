#ifndef __EVENT_TOUCH_H__
#define __EVENT_TOUCH_H__

#include "base/event/Event.h"
#include <vector>

/**
 * @addtogroup event
 * @{
 */
NS_BEGIN

class Touch;

class DLL EventTouch : public Event
{
public:
    static const int MAX_TOUCHES = 15;

    /** EventCode Touch event code.*/
    enum class EventCode
    {
        BEGAN,
        MOVED,
        ENDED,
        CANCELLED
    };

    EventTouch();

    inline EventCode getEventCode() const { return _eventCode; };

    inline const std::vector<Touch*>& getTouches() const { return _touches; };


private:
    EventCode _eventCode;
    std::vector<Touch*> _touches;

    friend class GLView;
};

NS_END
/**
 end of event group
 @}
 */

#endif // !__EVENT_TOUCH_H__
