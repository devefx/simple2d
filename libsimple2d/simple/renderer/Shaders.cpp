#include "renderer/Shaders.h"

#define STRINGIFY(A)  #A

NS_BEGIN

#include "shader/position_uColor.frag"
#include "shader/position_uColor.vert"

#include "shader/positionColor.frag"
#include "shader/positionColor.vert"

#include "shader/positionColorTextureAsPointsize.vert"

#include "shader/positionTexture.frag"
#include "shader/positionTexture.vert"

#include "shader/positionTextureA8Color.frag"
#include "shader/positionTextureA8Color.vert"

#include "shader/positionTextureColor.frag"
#include "shader/positionTextureColor.vert"

#include "shader/positionTextureColor_noMVP.frag"
#include "shader/positionTextureColor_noMVP.vert"

#include "shader/positionTextureColorAlphaTest.frag"

#include "shader/positionTexture_uColor.frag"
#include "shader/positionTexture_uColor.vert"

#include "shader/positionColorLengthTexture.frag"
#include "shader/positionColorLengthTexture.vert"

#include "shader/UIGray.frag"

#include "shader/label.vert"
#include "shader/label_df.frag"
#include "shader/label_df_glow.frag"
#include "shader/label_normal.frag"
#include "shader/label_outline.frag"



NS_END