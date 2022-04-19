//
//  PipelineDescriptor.h
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/4.
//

#ifndef PipelineDescriptor_h
#define PipelineDescriptor_h

#include "platform/AprilPlatformMacros.h"
#include "renderer/backend/DepthStencilState.h"
#include "renderer/backend/Texture.h"
#include "renderer/backend/VertexLayout.h"
#include "renderer/backend/RenderPassDescriptor.h"
#include "renderer/backend/ProgramState.h"

#include <string>

APRIL_BEGIN

struct PipelineDescriptor {
    backend::ProgramState*          program_state = nullptr;
    backend::BlendDescriptor        blend_descriptor;
};

APRIL_END


#endif /* PipelineDescriptor_h */
