//
//  RenderPipelineGL.hpp
//  april3d
//
//  Created by 黄世平 on 2022/4/3.
//

#ifndef RenderPipelineGL_hpp
#define RenderPipelineGL_hpp

#include <stdio.h>
#include <vector>

#include "platform/AprilGL.h"
#include "../RenderPipeline.h"
#include "../RenderPipelineDescriptor.h"


BACKEND_BEGIN

class ProgramGL;

class RenderPipelineGL : public RenderPipeline {
public:

    RenderPipelineGL() = default;
    ~RenderPipelineGL();

    virtual void update(const april3d::PipelineDescriptor & pipelineDescirptor, const RenderPassDescriptor& renderpassDescriptor) override;
  
    inline ProgramGL* getProgram() const { return program_gl; }

private:
    void updateBlendState(const BlendDescriptor& descriptor);

    ProgramGL* program_gl = nullptr;
};

BACKEND_END

#endif /* RenderPipelineGL_hpp */
