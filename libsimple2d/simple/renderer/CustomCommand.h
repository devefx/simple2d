#ifndef __CUSTOMCOMMAND_H__
#define __CUSTOMCOMMAND_H__

#include "renderer/RenderCommand.h"

#include <functional>

class DLL CustomCommand : public RenderCommand
{
public:
    void init(float globalZOrder, std::function<void()> renderFunc);

    void execute();

    inline virtual Type getType() const override
    {
        return RenderCommand::Type::CUSTOM_COMMAND;
    }
protected:

    std::function<void()> _renderFunc;
};

#endif // !__CUSTOMCOMMAND_H__
