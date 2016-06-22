#include "renderer/RenderQueue.h"

#include <algorithm>

static bool compare(const RenderCommand* a, const RenderCommand* b)
{
    return a->getGlobalOrder() > b->getGlobalOrder();
}

void RenderQueue::push_back(RenderCommand* command)
{
    float z = command->getGlobalOrder();
    if (z < 0)
    {
        _commands[GLOBALZ_NEG].push_back(command);
    }
    else if (z > 0)
    {
        _commands[GLOBALZ_POS].push_back(command);
    }
    else
    {
        _commands[GLOBALZ_ZERO].push_back(command);
    }
}

size_t RenderQueue::size() const
{
    size_t count = 0;
    for (int i = 0; i < QUEUE_COUNT; i++)
    {
        count += _commands[i].size();
    }
    return count;
}

void RenderQueue::sort()
{
    std::sort(std::begin(_commands[GLOBALZ_NEG]), std::end(_commands[GLOBALZ_NEG]), compare);
    std::sort(std::begin(_commands[GLOBALZ_POS]), std::end(_commands[GLOBALZ_POS]), compare);
}

void RenderQueue::clear()
{
    for (int i = 0; i < QUEUE_COUNT; i++)
    {
        _commands[i].clear();
    }
}

void RenderQueue::realloc(size_t reserveSize)
{
    for (int i = 0; i < QUEUE_COUNT; i++)
    {
        _commands[i] = std::vector<RenderCommand*>();
        _commands[i].reserve(reserveSize);
    }
}

void RenderQueue::saveRenderState()
{
    _isDepthEnabled = glIsEnabled(GL_DEPTH_TEST) != GL_FALSE;
    _isCullEnabled = glIsEnabled(GL_CULL_FACE) != GL_FALSE;
    glGetBooleanv(GL_DEPTH_WRITEMASK, &_isDepthWrite);

}

void RenderQueue::restoreRenderState()
{
    _isCullEnabled ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
    _isDepthEnabled ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
    glDepthMask(_isDepthWrite);

}
