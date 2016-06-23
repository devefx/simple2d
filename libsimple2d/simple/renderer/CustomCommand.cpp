#include "renderer/CustomCommand.h"


void CustomCommand::init(float globalZOrder, std::function<void()> renderFunc)
{
    RenderCommand::init(globalZOrder);
    _renderFunc = renderFunc;
}

void CustomCommand::execute()
{
    if (_renderFunc)
    {
        _renderFunc();
    }
}
