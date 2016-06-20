#include "renderer/GLProgram.h"

#include "renderer/GLStateCache.h"

NS_BEGIN

const char* GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR = "ShaderPositionTextureColor";
const char* GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP = "ShaderPositionTextureColor_noMVP";
const char* GLProgram::SHADER_NAME_POSITION_TEXTURE_ALPHA_TEST = "ShaderPositionTextureColorAlphaTest";
const char* GLProgram::SHADER_NAME_POSITION_TEXTURE_ALPHA_TEST_NO_MV = "ShaderPositionTextureColorAlphaTest_NoMV";
const char* GLProgram::SHADER_NAME_POSITION_COLOR = "ShaderPositionColor";
const char* GLProgram::SHADER_NAME_POSITION_COLOR_TEXASPOINTSIZE = "ShaderPositionColorTexAsPointsize";
const char* GLProgram::SHADER_NAME_POSITION_COLOR_NO_MVP = "ShaderPositionColor_noMVP";

const char* GLProgram::SHADER_NAME_POSITION_TEXTURE = "ShaderPositionTexture";
const char* GLProgram::SHADER_NAME_POSITION_TEXTURE_U_COLOR = "ShaderPositionTexture_uColor";
const char* GLProgram::SHADER_NAME_POSITION_TEXTURE_A8_COLOR = "ShaderPositionTextureA8Color";
const char* GLProgram::SHADER_NAME_POSITION_U_COLOR = "ShaderPosition_uColor";
const char* GLProgram::SHADER_NAME_POSITION_LENGTH_TEXTURE_COLOR = "ShaderPositionLengthTextureColor";
const char* GLProgram::SHADER_NAME_POSITION_GRAYSCALE = "ShaderUIGrayScale";
const char* GLProgram::SHADER_NAME_LABEL_DISTANCEFIELD_NORMAL = "ShaderLabelDFNormal";
const char* GLProgram::SHADER_NAME_LABEL_DISTANCEFIELD_GLOW = "ShaderLabelDFGlow";
const char* GLProgram::SHADER_NAME_LABEL_NORMAL = "ShaderLabelNormal";
const char* GLProgram::SHADER_NAME_LABEL_OUTLINE = "ShaderLabelOutline";

// uniform names
const char* GLProgram::UNIFORM_NAME_AMBIENT_COLOR = "CC_AmbientColor";
const char* GLProgram::UNIFORM_NAME_P_MATRIX = "CC_PMatrix";
const char* GLProgram::UNIFORM_NAME_MV_MATRIX = "CC_MVMatrix";
const char* GLProgram::UNIFORM_NAME_MVP_MATRIX  = "CC_MVPMatrix";
const char* GLProgram::UNIFORM_NAME_NORMAL_MATRIX = "CC_NormalMatrix";
const char* GLProgram::UNIFORM_NAME_TIME = "CC_Time";
const char* GLProgram::UNIFORM_NAME_SIN_TIME = "CC_SinTime";
const char* GLProgram::UNIFORM_NAME_COS_TIME = "CC_CosTime";
const char* GLProgram::UNIFORM_NAME_RANDOM01 = "CC_Random01";
const char* GLProgram::UNIFORM_NAME_SAMPLER0 = "CC_Texture0";
const char* GLProgram::UNIFORM_NAME_SAMPLER1 = "CC_Texture1";
const char* GLProgram::UNIFORM_NAME_SAMPLER2 = "CC_Texture2";
const char* GLProgram::UNIFORM_NAME_SAMPLER3 = "CC_Texture3";
const char* GLProgram::UNIFORM_NAME_ALPHA_TEST_VALUE = "CC_alpha_value";

// Attribute names
const char* GLProgram::ATTRIBUTE_NAME_COLOR = "a_color";
const char* GLProgram::ATTRIBUTE_NAME_POSITION = "a_position";
const char* GLProgram::ATTRIBUTE_NAME_TEX_COORD = "a_texCoord";
const char* GLProgram::ATTRIBUTE_NAME_TEX_COORD1 = "a_texCoord1";
const char* GLProgram::ATTRIBUTE_NAME_TEX_COORD2 = "a_texCoord2";
const char* GLProgram::ATTRIBUTE_NAME_TEX_COORD3 = "a_texCoord3";
const char* GLProgram::ATTRIBUTE_NAME_NORMAL = "a_normal";
const char* GLProgram::ATTRIBUTE_NAME_BLEND_WEIGHT = "a_blendWeight";
const char* GLProgram::ATTRIBUTE_NAME_BLEND_INDEX = "a_blendIndex";



GLProgram* GLProgram::createWithByteArrays(const GLchar* vShaderByteArray, const GLchar* fShaderByteArray)
{
    auto ret = new (std::nothrow) GLProgram();
    if (ret && ret->initWithByteArrays(vShaderByteArray, fShaderByteArray))
    {
        ret->link();
        ret->updateUniforms();
        ret->autorelease();
        return ret;
    }
    
    SAFE_DELETE(ret);
    return nullptr;
}

GLProgram* GLProgram::createWithFilenames(const std::string& vShaderFilename, const std::string& fShaderFilename)
{
    auto ret = new (std::nothrow) GLProgram();
    if (ret && ret->initWithFilenames(vShaderFilename, fShaderFilename))
    {
        ret->link();
        ret->updateUniforms();
        ret->autorelease();
    }

    SAFE_DELETE(ret);
    return nullptr;
}

GLProgram::GLProgram()
: _program(0)
, _vertShader(0)
, _fragShader(0)
, _flags()
{
    _director = Director::getInstance();
    SIMPLE2D_ASSERT(nullptr != _director, "Director is null when init a GLProgram");
    memset(_builtInUniforms, 0, sizeof(_builtInUniforms));
}

GLProgram::~GLProgram()
{
    LOGINFO("%s %d deallocing GLProgram: %p", __FUNCTION__, __LINE__, this);

    if (_vertShader)
    {
        glDeleteShader(_vertShader);
        _vertShader = 0;
    }
    if (_fragShader)
    {
        glDeleteShader(_fragShader);
        _fragShader = 0;
    }

    if (_program)
    {
        GL::deleteProgram(_program);
    }

    for (auto e : _hashForUniforms)
    {
        SAFE_FREE(e.second.first);
    }

    _hashForUniforms.clear();
}

bool GLProgram::initWithByteArrays(const GLchar* vShaderByteArray, const GLchar* fShaderByteArray)
{
    _program = glCreateProgram();
    CHECK_GL_ERROR_DEBUG();

    _vertShader = _fragShader = 0;

    if (vShaderByteArray)
    {
        if (!compileShader(&_vertShader, GL_VERTEX_SHADER, vShaderByteArray))
        {
            LOG("ERROR: Failed to compile vertex shader");
            return false;
        }
    }

    if (fShaderByteArray)
    {
        if (!compileShader(&_fragShader, GL_VERTEX_SHADER, fShaderByteArray))
        {
            LOG("ERROR: Failed to compile fragment shader");
            return false;
        }
    }

    if (_vertShader)
    {
        glAttachShader(_program, _vertShader);
    }
    CHECK_GL_ERROR_DEBUG();

    if (_fragShader)
    {
        glAttachShader(_program, _fragShader);
    }
    CHECK_GL_ERROR_DEBUG();

    _hashForUniforms.clear();

    return true;
}

bool GLProgram::initWithFilenames(const std::string& vShaderFilename, const std::string& fShaderFilename)
{
    
    return false;
}

void GLProgram::bindPredefinedVertexAttribs()
{
    glBindAttribLocation(_program, GLProgram::VERTEX_ATTRIB_POSITION,   GLProgram::ATTRIBUTE_NAME_POSITION);
    glBindAttribLocation(_program, GLProgram::VERTEX_ATTRIB_COLOR,      GLProgram::ATTRIBUTE_NAME_COLOR);
    glBindAttribLocation(_program, GLProgram::VERTEX_ATTRIB_TEX_COORD,  GLProgram::ATTRIBUTE_NAME_TEX_COORD);
    glBindAttribLocation(_program, GLProgram::VERTEX_ATTRIB_TEX_COORD1, GLProgram::ATTRIBUTE_NAME_TEX_COORD1);
    glBindAttribLocation(_program, GLProgram::VERTEX_ATTRIB_TEX_COORD2, GLProgram::ATTRIBUTE_NAME_TEX_COORD2);
    glBindAttribLocation(_program, GLProgram::VERTEX_ATTRIB_TEX_COORD3, GLProgram::ATTRIBUTE_NAME_TEX_COORD3);
    glBindAttribLocation(_program, GLProgram::VERTEX_ATTRIB_NORMAL,     GLProgram::ATTRIBUTE_NAME_NORMAL);
}

void GLProgram::parseVertexAttribs()
{
    _vertexAttribs.clear();

    // Query and store vertex attribute meta-data from the program.
    GLint activeAttributes;
    GLint length;
    glGetProgramiv(_program, GL_ACTIVE_ATTRIBUTES, &activeAttributes);
    if(activeAttributes > 0)
    {
        VertexAttrib attribute;

        glGetProgramiv(_program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &length);
        if(length > 0)
        {
            GLchar* attribName = (GLchar*) alloca(length + 1);

            for(int i = 0; i < activeAttributes; ++i)
            {
                // Query attribute info.
                glGetActiveAttrib(_program, i, length, nullptr, &attribute.size, &attribute.type, attribName);
                attribName[length] = '\0';
                attribute.name = std::string(attribName);

                // Query the pre-assigned attribute location
                attribute.index = glGetAttribLocation(_program, attribName);
                _vertexAttribs[attribute.name] = attribute;
            }
        }
    }
    else
    {
        GLchar ErrorLog[1024];
        glGetProgramInfoLog(_program, sizeof(ErrorLog), NULL, ErrorLog);
        LOG("Error linking shader program: '%s'\n", ErrorLog);
    }
}

void GLProgram::parseUniforms()
{
    _userUniforms.clear();

    // Query and store uniforms from the program.
    GLint activeUniforms;
    GLint length;
    glGetProgramiv(_program, GL_ACTIVE_UNIFORMS, &activeUniforms);
    if(activeUniforms > 0)
    {
        Uniform uniform;

        glGetProgramiv(_program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &length);
        if(length > 0)
        {
            GLchar* uniformName = (GLchar*) alloca(length + 1);

            for(int i = 0; i < activeUniforms; ++i)
            {
                // Query uniform info.
                glGetActiveUniform(_program, i, length, nullptr, &uniform.size, &uniform.type, uniformName);
                uniformName[length] = '\0';
                uniform.name = std::string(uniformName);

                // Query the pre-assigned uniform location
                uniform.location = glGetUniformLocation(_program, uniformName);
                _userUniforms[uniform.name] = uniform;
            }
        }
    }
    else
    {
        GLchar ErrorLog[1024];
        glGetProgramInfoLog(_program, sizeof(ErrorLog), NULL, ErrorLog);
        LOG("Error linking shader program: '%s'\n", ErrorLog);
    }
}

Uniform* GLProgram::getUniform(const std::string& name)
{
    const auto itr = _userUniforms.find(name);
    if (itr != _userUniforms.end())
    {
        return &itr->second;
    }
    return nullptr;
}

VertexAttrib* GLProgram::getVertexAttrib(const std::string& name)
{
    const auto itr = _vertexAttribs.find(name);
    if (itr != _vertexAttribs.end())
    {
        return &itr->second;
    }
    return nullptr;
}

bool GLProgram::compileShader(GLuint * shader, GLenum type, const GLchar* source)
{
    if (source)
    {
        GLint status;

        const GLchar *sources[] = {
            "uniform mat4 PMatrix;\n"
            "uniform mat4 MVMatrix;\n"
            "uniform mat4 MVPMatrix;\n"
            "uniform mat3 NormalMatrix;\n"
            "uniform vec4 Time;\n"
            "uniform vec4 SinTime;\n"
            "uniform vec4 CosTime;\n"
            "uniform vec4 Random01;\n"
            "uniform sampler2D Texture0;\n"
            "uniform sampler2D Texture1;\n"
            "uniform sampler2D Texture2;\n"
            "uniform sampler2D Texture3;\n",
            source
        };

        *shader = glCreateShader(type);
        glShaderSource(*shader, sizeof(sources) / sizeof(*sources), sources, nullptr);
        glCompileShader(*shader);

        glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);

        if (!status)
        {
            GLsizei length;
            glGetShaderiv(*shader, GL_SHADER_SOURCE_LENGTH, &length);
            GLchar* src = (GLchar*) malloc(sizeof(GLchar) * length);
            glGetShaderSource(*shader, length, nullptr, src);

            LOG("ERROR: Failed to compile shader: \n%s", src);
            if (type == GL_VERTEX_SHADER)
            {
                LOG("simple2d: %s", getVertexShaderLog().c_str());
            }
            else
            {
                LOG("simple2d: %s", getFragmentShaderLog().c_str());
            }
            SAFE_FREE(src);

            return false;
        }
        return status == GL_TRUE;
    }
    return false;
}

void GLProgram::bindAttribLocation(const std::string& attributeName, GLuint index) const
{
    glBindAttribLocation(_program, index, attributeName.c_str());
}

GLint GLProgram::getAttribLocation(const std::string& attributeName) const
{
    return glGetAttribLocation(_program, attributeName.c_str());
}

GLint GLProgram::getUniformLocation(const std::string& attributeName) const
{
    return glGetUniformLocation(_program, attributeName.c_str());
}

bool GLProgram::link()
{
    SIMPLE2D_ASSERT(_program != 0, "Cannot link invalid program");

    bindPredefinedVertexAttribs();

    glLinkProgram(_program);

    parseVertexAttribs();
    parseUniforms();

    if (_vertShader)
    {
        glDeleteShader(_vertShader);
    }

    if (_fragShader)
    {
        glDeleteShader(_fragShader);
    }

    _vertShader = _fragShader = 0;

    GLint status = GL_TRUE;

#if SIMPLE2D_DEBUG
    glGetProgramiv(_program, GL_LINK_STATUS, &status);

    if (status == GL_FALSE)
    {
        LOG("ERROR: Failed to link program: %i", _program);
        GL::deleteProgram(_program);
        _program = 0;
    }
#endif
    return true;
}

void GLProgram::use()
{
    GL::useProgram(_program);
}

void GLProgram::updateUniforms()
{
    _builtInUniforms[UNIFORM_AMBIENT_COLOR] = glGetUniformLocation(_program, UNIFORM_NAME_AMBIENT_COLOR);
    _builtInUniforms[UNIFORM_P_MATRIX] = glGetUniformLocation(_program, UNIFORM_NAME_P_MATRIX);
    _builtInUniforms[UNIFORM_MV_MATRIX] = glGetUniformLocation(_program, UNIFORM_NAME_MV_MATRIX);
    _builtInUniforms[UNIFORM_MVP_MATRIX] = glGetUniformLocation(_program, UNIFORM_NAME_MVP_MATRIX);
    _builtInUniforms[UNIFORM_NORMAL_MATRIX] = glGetUniformLocation(_program, UNIFORM_NAME_NORMAL_MATRIX);

    _builtInUniforms[UNIFORM_TIME] = glGetUniformLocation(_program, UNIFORM_NAME_TIME);
    _builtInUniforms[UNIFORM_SIN_TIME] = glGetUniformLocation(_program, UNIFORM_NAME_SIN_TIME);
    _builtInUniforms[UNIFORM_COS_TIME] = glGetUniformLocation(_program, UNIFORM_NAME_COS_TIME);

    _builtInUniforms[UNIFORM_RANDOM01] = glGetUniformLocation(_program, UNIFORM_NAME_RANDOM01);

    _builtInUniforms[UNIFORM_SAMPLER0] = glGetUniformLocation(_program, UNIFORM_NAME_SAMPLER0);
    _builtInUniforms[UNIFORM_SAMPLER1] = glGetUniformLocation(_program, UNIFORM_NAME_SAMPLER1);
    _builtInUniforms[UNIFORM_SAMPLER2] = glGetUniformLocation(_program, UNIFORM_NAME_SAMPLER2);
    _builtInUniforms[UNIFORM_SAMPLER3] = glGetUniformLocation(_program, UNIFORM_NAME_SAMPLER3);

    _flags.usesP = _builtInUniforms[UNIFORM_P_MATRIX] != -1;
    _flags.usesMV = _builtInUniforms[UNIFORM_MV_MATRIX] != -1;
    _flags.usesMVP = _builtInUniforms[UNIFORM_MVP_MATRIX] != -1;
    _flags.usesNormal = _builtInUniforms[UNIFORM_NORMAL_MATRIX] != -1;
    _flags.usesTime = (
                       _builtInUniforms[UNIFORM_TIME] != -1 ||
                       _builtInUniforms[UNIFORM_SIN_TIME] != -1 ||
                       _builtInUniforms[UNIFORM_COS_TIME] != -1
                       );
    _flags.usesRandom = _builtInUniforms[UNIFORM_RANDOM01] != -1;

    this->use();

    // Since sample most probably won't change, set it to 0,1,2,3 now.
    if(_builtInUniforms[UNIFORM_SAMPLER0] != -1)
        setUniformLocationWith1i(_builtInUniforms[UNIFORM_SAMPLER0], 0);
    if(_builtInUniforms[UNIFORM_SAMPLER1] != -1)
        setUniformLocationWith1i(_builtInUniforms[UNIFORM_SAMPLER1], 1);
    if(_builtInUniforms[UNIFORM_SAMPLER2] != -1)
        setUniformLocationWith1i(_builtInUniforms[UNIFORM_SAMPLER2], 2);
    if(_builtInUniforms[UNIFORM_SAMPLER3] != -1)
        setUniformLocationWith1i(_builtInUniforms[UNIFORM_SAMPLER3], 3);
}

static std::string logForOpenGLShader(GLuint shader)
{
    std::string ret;
    GLint logLength = 0, charsWritten = 0;

    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength < 1)
        return "";

    char *logBytes = (char*) malloc(logLength + 1);
    glGetShaderInfoLog(shader, logLength, &charsWritten, logBytes);
    logBytes[logLength] = '\0';
    ret = logBytes;

    free(logBytes);
    return ret;
}

static std::string logForOpenGLProgram(GLuint program)
{
    std::string ret;
    GLint logLength = 0, charsWritten = 0;

    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength < 1)
        return "";

    char *logBytes = (char*)malloc(logLength + 1);
    glGetProgramInfoLog(program, logLength, &charsWritten, logBytes);
    logBytes[logLength] = '\0';
    ret = logBytes;

    free(logBytes);
    return ret;
}

std::string GLProgram::getVertexShaderLog() const
{
    return logForOpenGLShader(_vertShader);
}

std::string GLProgram::getFragmentShaderLog() const
{
    return logForOpenGLShader(_fragShader);
}

std::string GLProgram::getProgramLog() const
{
    return logForOpenGLProgram(_program);
}

// Uniform cache

bool GLProgram::updateUniformLocation(GLint location, const GLvoid* data, unsigned int bytes)
{
    if (location < 0)
    {
        return false;
    }

    bool updated = true;

    auto element = _hashForUniforms.find(location);
    if (element == _hashForUniforms.end())
    {
        GLvoid* value = malloc(bytes);
        memcpy(value, data, bytes );
        _hashForUniforms.insert(std::make_pair(location, std::make_pair(value, bytes)));
    }
    else
    {
        if (memcmp(element->second.first, data, bytes) == 0)
        {
            updated = false;
        }
        else
        {
            if (element->second.second < bytes)
            {
                GLvoid* value = realloc(element->second.first, bytes);
                memcpy(value, data, bytes );
                _hashForUniforms[location] = std::make_pair(value, bytes);
            }
            else
                memcpy(element->second.first, data, bytes);
        }
    }

    return updated;
}

GLint GLProgram::getUniformLocationForName(const char* name) const
{
    SIMPLE2D_ASSERT(name != nullptr, "Invalid uniform name" );
    SIMPLE2D_ASSERT(_program != 0, "Invalid operation. Cannot get uniform location when program is not initialized");

    return glGetUniformLocation(_program, name);
}

/**
 @begin setUniformLocation
 {
 */
void GLProgram::setUniformLocationWith1i(GLint location, GLint i1)
{
    bool updated = updateUniformLocation(location, &i1, sizeof(i1)*1);

    if( updated )
    {
        glUniform1i((GLint)location, i1);
    }
}

//

void GLProgram::setUniformLocationWithMatrix4fv(GLint location, const GLfloat* matrixArray, unsigned int numberOfMatrices)
{
    bool updated = updateUniformLocation(location, matrixArray, sizeof(float)*16*numberOfMatrices);

    if( updated )
    {
        glUniformMatrix4fv((GLint)location, (GLsizei)numberOfMatrices, GL_FALSE, matrixArray);
    }
}

/**
 @end setUniformLocation
 }
 */

void GLProgram::setUniformsForBuiltins()
{
    setUniformsForBuiltins(Matrix::getInstance()->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW));
}

void GLProgram::setUniformsForBuiltins(const Mat4 &matrixMV)
{
    auto& matrixP = Matrix::getInstance()->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);

    if(_flags.usesP)
        setUniformLocationWithMatrix4fv(_builtInUniforms[UNIFORM_P_MATRIX], matrixP.m, 1);

    if(_flags.usesMV)
        setUniformLocationWithMatrix4fv(_builtInUniforms[UNIFORM_MV_MATRIX], matrixMV.m, 1);

    if(_flags.usesMVP) {
        Mat4 matrixMVP = matrixP * matrixMV;
        setUniformLocationWithMatrix4fv(_builtInUniforms[UNIFORM_MVP_MATRIX], matrixMVP.m, 1);
    }

    if (_flags.usesNormal)
    {
        
    }

    if(_flags.usesTime)
    {
        
    }

    if(_flags.usesRandom) 
    {

    }
}

void GLProgram::reset()
{
    _vertShader = _fragShader = 0;
    memset(_builtInUniforms, 0, sizeof(_builtInUniforms));

    // it is already deallocated by android
    //GL::deleteProgram(_program);
    _program = 0;

    for (auto e: _hashForUniforms)
    {
        free(e.second.first);
    }

    _hashForUniforms.clear();
}


NS_END