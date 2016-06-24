#ifndef __RENDER_COMMAND_H__
#define __RENDER_COMMAND_H__

#include <stdint.h>

// temp
#include "platform/PlatformMacros.h"

NS_BEGIN

class DLL RenderCommand
{
public:
    enum class Type
    {
        QUAD_COMMAND,
        CUSTOM_COMMAND
    };

    void init(float globalZOrder);

    inline float getGlobalOrder() const { return _globalOrder; }

    virtual Type getType() const = 0;

    inline bool isSkipBatching() const { return _skipBatching; }

    inline void setSkipBatching(bool isSkipBatching) { _skipBatching = isSkipBatching; }

protected:
    RenderCommand();

    ~RenderCommand();

    float _globalOrder;

    bool _skipBatching;
};

NS_END

#endif // !__RENDER_COMMAND_H__
