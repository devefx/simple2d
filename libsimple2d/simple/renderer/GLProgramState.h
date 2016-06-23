#ifndef __GLPROGRAMSTATE_H__
#define __GLPROGRAMSTATE_H__

#include "renderer/GLProgram.h"

#include "math/Mat4.h"

class DLL GLProgramState
{
public:
    static GLProgramState* create(GLProgram* glprogram);



    void apply(const Mat4& modelView);

    GLProgram* getGLProgram() const { return _glprogram; }

protected:
    GLProgramState();
    ~GLProgramState();
    bool init(GLProgram* glprogram);


    GLProgram* _glprogram;


};

#endif // !__GLPROGRAMSTATE_H__
