#ifndef __SHADERS_H__
#define __SHADERS_H__

#include "platform/GL.h"
#include "platform/PlatformMacros.h"

/**
 * @addtogroup renderer
 * @{
 */
NS_BEGIN

extern DLL const GLchar * position_uColor_frag;
extern DLL const GLchar * position_uColor_vert;

extern DLL const GLchar * positionColor_frag;
extern DLL const GLchar * positionColor_vert;

extern DLL const GLchar * positionColorTextureAsPointsize_vert;

extern DLL const GLchar * positionTexture_frag;
extern DLL const GLchar * positionTexture_vert;

extern DLL const GLchar * positionTextureA8Color_frag;
extern DLL const GLchar * positionTextureA8Color_vert;

extern DLL const GLchar * positionTextureColor_frag;
extern DLL const GLchar * positionTextureColor_vert;

extern DLL const GLchar * positionTextureColor_noMVP_frag;
extern DLL const GLchar * positionTextureColor_noMVP_vert;

extern DLL const GLchar * positionTextureColorAlphaTest_frag;

extern DLL const GLchar * positionTexture_uColor_frag;
extern DLL const GLchar * positionTexture_uColor_vert;

extern DLL const GLchar * positionColorLengthTexture_frag;
extern DLL const GLchar * positionColorLengthTexture_vert;

extern DLL const GLchar * positionTexture_GrayScale_frag;

extern DLL const GLchar * labelDistanceFieldNormal_frag;
extern DLL const GLchar * labelDistanceFieldGlow_frag;
extern DLL const GLchar * labelNormal_frag;
extern DLL const GLchar * labelOutline_frag;

extern DLL const GLchar * label_vert;

NS_END
/**
 end of renderer group
 @}
 */

#endif // !__SHADERS_H__
