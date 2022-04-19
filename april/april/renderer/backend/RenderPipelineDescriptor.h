//
//  RenderPipelineDescriptor.h
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/2.
//

#ifndef RenderPipelineDescriptor_h
#define RenderPipelineDescriptor_h

#include "Macros.h"
#include "Types.h"
#include "VertexLayout.h"
#include "renderer/backend/ProgramState.h"

#include <vector>
#include <memory>

BACKEND_BEGIN

class DepthStencilState;
class BlendState;

struct RenderPipelineDescriptor {
    ProgramState* programState = nullptr;
    BlendState* blendState = nullptr;
    PixelFormat colorAttachmentsFormat[MAX_COLOR_ATTCHMENT] = { PixelFormat::DEFAULT };
    PixelFormat depthAttachmentFormat = PixelFormat::NONE;
    PixelFormat stencilAttachmentFormat = PixelFormat::NONE;
};

BACKEND_END



#endif /* RenderPipelineDescriptor_h */
