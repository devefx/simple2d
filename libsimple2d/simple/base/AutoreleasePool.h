#ifndef __BASE_AUTORELEASE_POOL_H__
#define __BASE_AUTORELEASE_POOL_H__

#include <vector>
#include <string>
#include "base/Ref.h"

/**
 * @addtogroup base
 * @{
 */
NS_BEGIN

class DLL AutoreleasePool
{
public:
    AutoreleasePool();

    AutoreleasePool(const std::string &name);

    ~AutoreleasePool();

    void addObject(Ref *object);

    void clear();

    bool contains(Ref* object) const;

private:
    std::vector<Ref*> _managedObjectArray;
    std::string _name;
};

class DLL PoolManager
{
public:
    static PoolManager* getInstance();

    static void destroyInstance();

    AutoreleasePool *getCurrentPool() const;

    bool isObjectInPools(Ref* obj) const;

    friend class AutoreleasePool;

private:
    PoolManager();

    ~PoolManager();

    void push(AutoreleasePool *pool);
    void pop();

    static PoolManager* s_singleInstance;

    std::vector<AutoreleasePool*> _releasePoolStack;
};

NS_END
// end of base group
/** @} */

#endif // !__BASE_AUTORELEASE_POOL_H__
