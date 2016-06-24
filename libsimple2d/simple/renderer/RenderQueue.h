#ifndef __RENDER_QUEUE_H__
#define __RENDER_QUEUE_H__

#include <vector>

#include "renderer/RenderCommand.h"

//temp
#include "platform/GL.h"

NS_BEGIN

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
    /** ���һ����Ⱦ�����ǰ��Ⱦ���� */
    void push_back(RenderCommand* command);
    /** ������Ⱦ�������� */
    size_t size() const;
    /** ������Ⱦ���� */
    void sort();
    /** �����Ⱦ���� */
    void clear();
    /** ���·�����Ⱦ���д�С */
    void realloc(size_t reserveSize);
    /** ��ȡһ����Ⱦ����Ƭ�� */
    inline std::vector<RenderCommand*>& getSubQueue(QUEUE_GROUP group) { return _commands[group]; }
    /** ��ȡһ����Ⱦ����Ƭ�ε���Ⱦ�������� */
    inline size_t getSubQueueSize(QUEUE_GROUP group) const { return _commands[group].size();}
    /** ������Ⱦ״̬ */
    void saveRenderState();
    /** �ָ���Ⱦ״̬ */
    void restoreRenderState();
protected:
    std::vector<RenderCommand*> _commands[QUEUE_COUNT];

    bool _isCullEnabled;
    bool _isDepthEnabled;
    GLboolean _isDepthWrite;
};

NS_END

#endif // !__RENDER_QUEUE_H__
