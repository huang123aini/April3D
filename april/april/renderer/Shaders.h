//
//  Shaders.hpp
//  april3d
//
//  Created by 黄世平 on 2022/4/4.
//

#ifndef Shaders_hpp
#define Shaders_hpp

#include <stdio.h>


#include "platform/AprilPlatformMacros.h"

APRIL_BEGIN

extern  const char * positionColor_vert;
extern  const char * positionColor_frag;
extern  const char * positionTexture_vert;
extern  const char * positionTexture_frag;
extern  const char * positionTextureColor_vert;
extern  const char * positionTextureColor_frag;
extern  const char * positionTextureColorAlphaTest_frag;
extern  const char * label_normal_frag;
extern  const char * label_distanceNormal_frag;
extern  const char * labelOutline_frag;
extern  const char * labelDistanceFieldGlow_frag;
extern  const char * lineColor3D_frag;
extern  const char * lineColor3D_vert;
extern  const char * positionColorLengthTexture_vert;
extern  const char * positionColorLengthTexture_frag;
extern  const char * positionColorTextureAsPointsize_vert;
extern  const char * position_vert;
extern  const char * positionNoMVP_vert;
extern  const char * layer_radialGradient_frag;
extern  const char * grayScale_frag;
extern  const char * positionTextureUColor_vert;
extern  const char * positionTextureUColor_frag;
extern  const char * positionUColor_vert;
extern  const char * positionUColor_frag;
extern  const char * etc1_frag;
extern  const char * etc1Gray_frag;
extern  const char * cameraClear_vert;
extern  const char * cameraClear_frag;

extern  const char * CC3D_color_frag;
extern  const char * CC3D_colorNormal_frag;
extern  const char * CC3D_colorNormalTexture_frag;
extern  const char * CC3D_colorTexture_frag;
extern  const char * CC3D_particleTexture_frag;
extern  const char * CC3D_particleColor_frag;
extern  const char * CC3D_particle_vert;
extern  const char * CC3D_positionNormalTexture_vert;
extern  const char * CC3D_skinPositionNormalTexture_vert;
extern  const char * CC3D_positionTexture_vert;
extern  const char * CC3D_skinPositionTexture_vert;
extern  const char * CC3D_skybox_frag;
extern  const char * CC3D_skybox_vert;
extern  const char * CC3D_terrain_frag;
extern  const char * CC3D_terrain_vert;

APRIL_END

#endif /* Shaders_hpp */
