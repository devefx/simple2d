#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "platform/PlatformMacros.h"
#include "platform/GL.h"

/**
 * @addtogroup renderer
 * @{
 */
NS_BEGIN



class DLL Renderer
{
public:
    /**The max number of vertices in a vertex buffer object.*/
    static const int VBO_SIZE = 65536;
    /**The max numer of indices in a index buffer.*/
    static const int INDEX_VBO_SIZE = VBO_SIZE * 6 / 4;
    /**The rendercommands which can be batched will be saved into a list, this is the reversed size of this list.*/
    static const int BATCH_QUADCOMMAND_RESEVER_SIZE = 64;
    /**Reserved for material id, which means that the command could not be batched.*/
    static const int MATERIAL_ID_DO_NOT_BATCH = 0;

    Renderer();

    ~Renderer();

    void initGLView();

    //

    void render();

    void clean();

    void clear();

protected:



};


NS_END
/**
 end of renderer group
 @}
 */

#endif // !__RENDERER_H__
