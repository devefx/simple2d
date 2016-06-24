#include "renderer/RenderCommand.h"

NS_BEGIN

RenderCommand::RenderCommand()
    : _globalOrder(0)
    , _skipBatching(false)
{

}

RenderCommand::~RenderCommand()
{

}

void RenderCommand::init(float globalZOrder)
{
    _globalOrder = globalZOrder;
}

NS_END