#include "renderer/GLProgramCache.h"

#include "renderer/Shaders.h"
#include "base/Macros.h"

NS_BEGIN

enum {
    kShaderType_PositionTextureColor,
    kShaderType_PositionTextureColor_noMVP,
    kShaderType_PositionTextureColorAlphaTest,
    kShaderType_PositionTextureColorAlphaTestNoMV,
    kShaderType_PositionColor,
    kShaderType_PositionColorTextureAsPointsize,
    kShaderType_PositionColor_noMVP,
    kShaderType_PositionTexture,
    kShaderType_PositionTexture_uColor,
    kShaderType_PositionTextureA8Color,
    kShaderType_Position_uColor,
    kShaderType_PositionLengthTexureColor,
    kShaderType_LabelDistanceFieldNormal,
    kShaderType_LabelDistanceFieldGlow,
    kShaderType_UIGrayScale,
    kShaderType_LabelNormal,
    kShaderType_LabelOutline,

    kShaderType_MAX,
};

static GLProgramCache *_sharedGLProgramCache = 0;

GLProgramCache* GLProgramCache::getInstance()
{
    if (!_sharedGLProgramCache) {
        _sharedGLProgramCache = new (std::nothrow) GLProgramCache();
        if (!_sharedGLProgramCache->init())
        {
            SAFE_DELETE(_sharedGLProgramCache);
        }
    }
    return _sharedGLProgramCache;
}

void GLProgramCache::destroyInstance()
{
    SAFE_RELEASE_NULL(_sharedGLProgramCache);
}

GLProgramCache::GLProgramCache()
    : _programs()
{

}

GLProgramCache::~GLProgramCache()
{
    for( auto it = _programs.begin(); it != _programs.end(); ++it ) {
        (it->second)->release();
    }
    LOGINFO("deallocing GLProgramCache: %p", this);
}

bool GLProgramCache::init()
{
    loadDefaultGLPrograms();
    return true;
}

//------------------------------------------------------------------------------
//
// 
//
//------------------------------------------------------------------------------

#define loadGLProgram(type, name) \
    p = new (std::nothrow) GLProgram(); \
    loadDefaultGLProgram(p, type); \
    _programs.insert(std::make_pair( GLProgram::name, p ));

#define reloadGLProgram(type, name) \
    p = getGLProgram(GLProgram::name); \
    p->reset(); \
    loadDefaultGLProgram(p, type);

void GLProgramCache::loadDefaultGLPrograms()
{
    GLProgram *p;

    // Position Texture Color shader
    loadGLProgram(kShaderType_PositionTextureColor,                 SHADER_NAME_POSITION_TEXTURE_COLOR);

    // Position Texture Color without MVP shader
    loadGLProgram(kShaderType_PositionTextureColor_noMVP,           SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP);

    // Position Texture Color alpha test
    loadGLProgram(kShaderType_PositionTextureColorAlphaTest,        SHADER_NAME_POSITION_TEXTURE_ALPHA_TEST);

    // Position Texture Color alpha test
    loadGLProgram(kShaderType_PositionTextureColorAlphaTestNoMV,    SHADER_NAME_POSITION_TEXTURE_ALPHA_TEST_NO_MV);

    //
    // Position, Color shader
    //
    loadGLProgram(kShaderType_PositionColor,                        SHADER_NAME_POSITION_COLOR);

    // Position, Color, PointSize shader
    loadGLProgram(kShaderType_PositionColorTextureAsPointsize,      SHADER_NAME_POSITION_COLOR_TEXASPOINTSIZE);

    //
    // Position, Color shader no MVP
    //
    loadGLProgram(kShaderType_PositionColor_noMVP,                  SHADER_NAME_POSITION_COLOR_NO_MVP);

    //
    // Position Texture shader
    //
    loadGLProgram(kShaderType_PositionTexture,                      SHADER_NAME_POSITION_TEXTURE);

    //
    // Position, Texture attribs, 1 Color as uniform shader
    //
    loadGLProgram(kShaderType_PositionTexture_uColor,               SHADER_NAME_POSITION_TEXTURE_U_COLOR);

    //
    // Position Texture A8 Color shader
    //
    loadGLProgram(kShaderType_PositionTextureA8Color,               SHADER_NAME_POSITION_TEXTURE_A8_COLOR);

    //
    // Position and 1 color passed as a uniform (to simulate glColor4ub )
    //
    loadGLProgram(kShaderType_Position_uColor,                      SHADER_NAME_POSITION_U_COLOR);

    //
    // Position, Legth(TexCoords, Color (used by Draw Node basically )
    //
    loadGLProgram(kShaderType_PositionLengthTexureColor,            SHADER_NAME_POSITION_LENGTH_TEXTURE_COLOR);

    loadGLProgram(kShaderType_UIGrayScale,                          SHADER_NAME_POSITION_GRAYSCALE);

    loadGLProgram(kShaderType_LabelNormal,                          SHADER_NAME_LABEL_NORMAL);

    loadGLProgram(kShaderType_LabelOutline,                         SHADER_NAME_LABEL_OUTLINE);
}

void GLProgramCache::reloadDefaultGLPrograms()
{
    GLProgram *p;

    // Position Texture Color shader
    reloadGLProgram(kShaderType_PositionTextureColor,               SHADER_NAME_POSITION_TEXTURE_COLOR);

    // Position Texture Color without MVP shader
    reloadGLProgram(kShaderType_PositionTextureColor_noMVP,         SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP);

    // Position Texture Color alpha test
    reloadGLProgram(kShaderType_PositionTextureColorAlphaTest,      SHADER_NAME_POSITION_TEXTURE_ALPHA_TEST);

    // Position Texture Color alpha test
    reloadGLProgram(kShaderType_PositionTextureColorAlphaTestNoMV,  SHADER_NAME_POSITION_TEXTURE_ALPHA_TEST_NO_MV);

    //
    // Position, Color shader
    //
    reloadGLProgram(kShaderType_PositionColor,                      SHADER_NAME_POSITION_COLOR);

    // Position, Color, PointSize shader
    reloadGLProgram(kShaderType_PositionColorTextureAsPointsize,    SHADER_NAME_POSITION_COLOR_TEXASPOINTSIZE);

    //
    // Position, Color shader no MVP
    //
    reloadGLProgram(kShaderType_PositionColor_noMVP,                SHADER_NAME_POSITION_COLOR_NO_MVP);

    //
    // Position Texture shader
    //
    reloadGLProgram(kShaderType_PositionTexture,                    SHADER_NAME_POSITION_TEXTURE);

    //
    // Position, Texture attribs, 1 Color as uniform shader
    //
    reloadGLProgram(kShaderType_PositionTexture_uColor,             SHADER_NAME_POSITION_TEXTURE_U_COLOR);

    //
    // Position Texture A8 Color shader
    //
    reloadGLProgram(kShaderType_PositionTextureA8Color,             SHADER_NAME_POSITION_TEXTURE_A8_COLOR);
    
    //
    // Position and 1 color passed as a uniform (to simulate glColor4ub )
    //
    reloadGLProgram(kShaderType_Position_uColor,                    SHADER_NAME_POSITION_U_COLOR);
    
    //
    // Position, Legth(TexCoords, Color (used by Draw Node basically )
    //
    reloadGLProgram(kShaderType_PositionLengthTexureColor,          SHADER_NAME_POSITION_LENGTH_TEXTURE_COLOR);
    
    reloadGLProgram(kShaderType_LabelNormal,                        SHADER_NAME_LABEL_NORMAL);
    
    reloadGLProgram(kShaderType_LabelOutline,                       SHADER_NAME_LABEL_OUTLINE);
}

void GLProgramCache::loadDefaultGLProgram(GLProgram *p, int type)
{
    switch (type) {
    case kShaderType_PositionTextureColor:
        p->initWithByteArrays(positionTextureColor_vert, positionTextureColor_frag);
        break;
    case kShaderType_PositionTextureColor_noMVP:
        p->initWithByteArrays(positionTextureColor_noMVP_vert, positionTextureColor_noMVP_frag);
        break;
    case kShaderType_PositionTextureColorAlphaTest:
        p->initWithByteArrays(positionTextureColor_vert, positionTextureColorAlphaTest_frag);
        break;
    case kShaderType_PositionTextureColorAlphaTestNoMV:
        p->initWithByteArrays(positionTextureColor_noMVP_vert, positionTextureColorAlphaTest_frag);
        break;
    case kShaderType_PositionColor:
        p->initWithByteArrays(positionColor_vert ,positionColor_frag);
        break;
    case kShaderType_PositionColorTextureAsPointsize:
        p->initWithByteArrays(positionColorTextureAsPointsize_vert ,positionColor_frag);
        break;
    case kShaderType_PositionColor_noMVP:
        p->initWithByteArrays(positionTextureColor_noMVP_vert ,positionColor_frag);
        break;
    case kShaderType_PositionTexture:
        p->initWithByteArrays(positionTexture_vert ,positionTexture_frag);
        break;
    case kShaderType_PositionTexture_uColor:
        p->initWithByteArrays(positionTexture_uColor_vert, positionTexture_uColor_frag);
        break;
    case kShaderType_PositionTextureA8Color:
        p->initWithByteArrays(positionTextureA8Color_vert, positionTextureA8Color_frag);
        break;
    case kShaderType_Position_uColor:
        p->initWithByteArrays(position_uColor_vert, position_uColor_frag);
        p->bindAttribLocation("aVertex", GLProgram::VERTEX_ATTRIB_POSITION);
        break;
    case kShaderType_PositionLengthTexureColor:
        p->initWithByteArrays(positionColorLengthTexture_vert, positionColorLengthTexture_frag);
        break;
    case kShaderType_UIGrayScale:
        p->initWithByteArrays(positionTextureColor_noMVP_vert,
            positionTexture_GrayScale_frag);
        break;
    case kShaderType_LabelNormal:
        p->initWithByteArrays(label_vert, labelNormal_frag);
        break;
    case kShaderType_LabelOutline:
        p->initWithByteArrays(label_vert, labelOutline_frag);
        break;
    default:
        LOG("simple2d: %s:%d, error shader type", __FUNCTION__, __LINE__);
        return;
    }

    p->link();
    p->updateUniforms();

    CHECK_GL_ERROR_DEBUG();
}

GLProgram* GLProgramCache::getGLProgram(const std::string &key)
{
    auto it = _programs.find(key);
    if( it != _programs.end() )
        return it->second;
    return nullptr;
}

void GLProgramCache::addGLProgram(GLProgram* program, const std::string &key)
{
    // release old one
    auto prev = getGLProgram(key);
    if( prev == program )
        return;

    _programs.erase(key);
    SAFE_RELEASE_NULL(prev);

    if (program)
        program->retain();
    _programs[key] = program;
}

NS_END