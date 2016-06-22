#ifndef __CUSTOMCOMMAND_H__
#define __CUSTOMCOMMAND_H__

#include "renderer/RenderCommand.h"

#include <functional>

class DLL CustomCommand : public RenderCommand
{
public:
    void init(float globalZOrder, std::function<void()> renderFunc);

    virtual Type getType() const override;

    void execute();

protected:

    std::function<void()> _renderFunc;
};

#endif // !__CUSTOMCOMMAND_H__
