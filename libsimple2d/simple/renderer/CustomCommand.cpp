#include "renderer/CustomCommand.h"


void CustomCommand::init(float globalZOrder, std::function<void()> renderFunc)
{
    RenderCommand::init(globalZOrder);
    _renderFunc = renderFunc;
}

RenderCommand::Type CustomCommand::getType() const 
{
    return RenderCommand::Type::CUSTOM_COMMAND;
}

void CustomCommand::execute()
{
    if (_renderFunc)
    {
        _renderFunc();
    }
}
