#ifndef __RENDER_QUEUE_H__
#define __RENDER_QUEUE_H__

#include <vector>

#include "renderer/RenderCommand.h"

//temp
#include "GL/glew.h"

class RenderQueue
{
public:
    enum QUEUE_GROUP
    {
        GLOBALZ_NEG,
        GLOBALZ_ZERO,
        GLOBALZ_POS,
        QUEUE_COUNT
    };
public:
    /** 添加一个渲染命令到当前渲染队列 */
    void push_back(RenderCommand* command);
    /** 返回渲染命令数量 */
    size_t size() const;
    /** 排序渲染队列 */
    void sort();
    /** 清空渲染队列 */
    void clear();
    /** 重新分配渲染队列大小 */
    void realloc(size_t reserveSize);
    /** 获取一个渲染队列片段 */
    inline std::vector<RenderCommand*>& getSubQueue(QUEUE_GROUP group) { return _commands[group]; }
    /** 获取一个渲染队列片段的渲染命令数量 */
    inline size_t getSubQueueSize(QUEUE_GROUP group) const { return _commands[group].size();}
    /** 保存渲染状态 */
    void saveRenderState();
    /** 恢复渲染状态 */
    void restoreRenderState();
protected:
    std::vector<RenderCommand*> _commands[QUEUE_COUNT];

    bool _isCullEnabled;
    bool _isDepthEnabled;
    GLboolean _isDepthWrite;
};



#endif // !__RENDER_QUEUE_H__
