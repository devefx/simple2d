#include "base/event/Event.h"

NS_BEGIN

Event::Event(Type type)
: _type(type)
, _isStopped(false)
{
}

Event::~Event()
{
}

NS_END