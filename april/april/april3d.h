#pragma once

/**
 *对外暴露的接口
 */


/**
 *@brief config相关
 */ 

#include "base/aprilConfig.h"

/**
 *@brief 绘制相关  
 */ 

#include "renderer/CCCallbackCommand.h"
#include "renderer/CCCustomCommand.h"
#include "renderer/CCGroupCommand.h"
#include "renderer/CCMaterial.h"
#include "renderer/CCPass.h"
#include "renderer/CCQuadCommand.h"
#include "renderer/CCRenderCommand.h"
#include "renderer/CCRenderCommandPool.h"
#include "renderer/CCRenderState.h"
#include "renderer/CCRenderer.h"
#include "renderer/CCTechnique.h"
#include "renderer/CCTexture2D.h"
#include "renderer/CCTextureCube.h"
#include "renderer/CCTextureCache.h"
#include "renderer/CCTrianglesCommand.h"
#include "renderer/ccShaders.h"


/**
 *@brief 平台相关  
 */ 

#include "platform/AprilPlatformConfig.h"
#include "platform/AprilPlatformMacros.h"

#if (APRIL_OS == APRIL_OS_IOS)
/**
 *ios相关
 */ 
#endif


#if (APRIL_OS == APRIL_OS_ANDROID)
/**
 *android相关
 */ 

#endif

#if (APRIL_OS == APRIL_OS_MAC)
/**
 *mac相关 
 */ 
#endif

#if (APRIL_OS == APRIL_OS_LINUX)
/**
 *linux相关 
 */ 
#endif

#if (APRIL_OS == APRIL_OS_WIN32)
/**
 *win32相关 
 */ 
#endif



