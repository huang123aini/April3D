//
//  RenderPipeline.h
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/2.
//

#ifndef RenderPipeline_h
#define RenderPipeline_h

#include "Macros.h"
#include "Types.h"
#include "base/Ref.h"
#include "renderer/PipelineDescriptor.h"
#include "renderer/backend/RenderPassDescriptor.h"

BACKEND_BEGIN

class RenderPipeline : public april3d::Ref {
public:
    virtual void update(const april3d::PipelineDescriptor & pipelineDescirptor, const RenderPassDescriptor& renderpassDescriptor) = 0;
protected:
    virtual ~RenderPipeline() = default;
};

BACKEND_END


#endif /* RenderPipeline_h */
