#ifndef __GLPROGRAMCAHCHE_H__
#define __GLPROGRAMCAHCHE_H__

#include <unordered_map>

#include "base/Ref.h"
#include "renderer/GLProgram.h"

/**
 * @addtogroup renderer
 * @{
 */
NS_BEGIN

class DLL GLProgramCache : public Ref
{
public:
    GLProgramCache();

    ~GLProgramCache();

    static GLProgramCache* getInstance();

    static void destroyInstance();

    void loadDefaultGLPrograms();

    void reloadDefaultGLPrograms();

    GLProgram* getGLProgram(const std::string &key);

    void addGLProgram(GLProgram* program, const std::string &key);

public:
    bool init();
    void loadDefaultGLProgram(GLProgram *program, int type);

    std::unordered_map<std::string, GLProgram*> _programs;
};

NS_END
/**
 end of renderer group
 @}
 */


#endif // !__GLPROGRAMCAHCHE_H__
