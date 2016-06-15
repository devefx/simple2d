#ifndef __BASE_MATRIX_CONTROL_H__
#define __BASE_MATRIX_CONTROL_H__

#include "math/Mat4.h"
#include <stack>

/**
 * @addtogroup base
 * @{
 */
NS_BEGIN

/**
 * @brief Matrix stack type.
 */
enum class MATRIX_STACK_TYPE
{
    /// Model view matrix stack
    MATRIX_STACK_MODELVIEW,

    /// projection matrix stack
    MATRIX_STACK_PROJECTION,

    /// texture matrix stack
    MATRIX_STACK_TEXTURE
};


class DLL MatrixControl
{
public:
    MatrixControl();

    ~MatrixControl();

    void initMatrixStack();

    void pushMatrix(MATRIX_STACK_TYPE type);

    void popMatrix(MATRIX_STACK_TYPE type);

    void loadIdentityMatrix(MATRIX_STACK_TYPE type);

    void loadMatrix(MATRIX_STACK_TYPE type, const Mat4& mat);

    void multiplyMatrix(MATRIX_STACK_TYPE type, const Mat4& mat);

    const Mat4& getMatrix(MATRIX_STACK_TYPE type);

    void resetMatrixStack();

protected:
    std::stack<Mat4> _modelViewMatrixStack;
    std::stack<Mat4> _projectionMatrixStack;
    std::stack<Mat4> _textureMatrixStack;

};

NS_END
/**
 end of base group
 @}
 */

#endif // !__BASE_MATRIX_CONTROL_H__
