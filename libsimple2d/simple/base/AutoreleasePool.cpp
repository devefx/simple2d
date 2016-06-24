#include "base/AutoreleasePool.h"

NS_BEGIN

AutoreleasePool::AutoreleasePool()
: _name("")
{
    _managedObjectArray.reserve(150);
    PoolManager::getInstance()->push(this);
}

AutoreleasePool::AutoreleasePool(const std::string &name)
: _name(name)
{
    _managedObjectArray.reserve(150);
    PoolManager::getInstance()->push(this);
}

AutoreleasePool::~AutoreleasePool()
{
    clear();
    PoolManager::getInstance()->pop();
}

void AutoreleasePool::addObject(Ref *object)
{
    _managedObjectArray.push_back(object);
}

void AutoreleasePool::clear()
{
    std::vector<Ref*> releasings;
    releasings.swap(_managedObjectArray);
    for (const auto &obj : releasings)
    {
        obj->release();
    }
}

bool AutoreleasePool::contains(Ref* object) const
{
    for (const auto &obj : _managedObjectArray)
    {
        if (obj == object)
        {
            return true;
        }
    }
    return false;
}

//--------------------------------------------------------------------
//
// PoolManager
//
//--------------------------------------------------------------------

PoolManager* PoolManager::s_singleInstance = nullptr;

PoolManager* PoolManager::getInstance()
{
    if (s_singleInstance == nullptr)
    {
        s_singleInstance = new (std::nothrow) PoolManager();
        new AutoreleasePool("simple2d autorelease pool");
    }
    return s_singleInstance;
}

void PoolManager::destroyInstance()
{
    delete s_singleInstance;
    s_singleInstance = nullptr;
}

PoolManager::PoolManager()
{
    _releasePoolStack.reserve(10);
}

PoolManager::~PoolManager()
{
    while (!_releasePoolStack.empty())
    {
        AutoreleasePool* pool = _releasePoolStack.back();

        delete pool;
    }
}

AutoreleasePool* PoolManager::getCurrentPool() const
{
    return _releasePoolStack.back();
}

bool PoolManager::isObjectInPools(Ref* obj) const
{
    for (const auto& pool : _releasePoolStack)
    {
        if (pool->contains(obj))
        {
            return true;
        }
    }
    return false;
}

void PoolManager::push(AutoreleasePool *pool)
{
    _releasePoolStack.push_back(pool);
}

void PoolManager::pop()
{
    ASSERT(!_releasePoolStack.empty());
    _releasePoolStack.pop_back();
}

NS_END