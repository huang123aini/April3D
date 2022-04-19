//
//  RenderPipelineGL.cpp
//  april3d
//
//  Created by 黄世平 on 2022/4/3.
//

#include <assert.h>
#include "RenderPipelineGL.h"
#include "ShaderModuleGL.h"
#include "DepthStencilStateGL.h"
#include "ProgramGL.h"
#include "UtilsGL.h"

#include "renderer/PipelineDescriptor.h"

#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>

BACKEND_BEGIN

void RenderPipelineGL::update(const april3d::PipelineDescriptor& pipelineDescirptor, const RenderPassDescriptor& renderpassDescriptor) {
    if(program_gl != pipelineDescirptor.program_state->getProgram()) {
        SAFE_RELEASE(program_gl);
        program_gl = static_cast<ProgramGL*>(pipelineDescirptor.program_state->getProgram());
        SAFE_RETAIN(program_gl);
    }
    updateBlendState(pipelineDescirptor.blend_descriptor);
}

void RenderPipelineGL::updateBlendState(const BlendDescriptor& descriptor) {
    auto blendEnabled = descriptor.blend_enabled;
    auto rgbBlendOperation = UtilsGL::toGLBlendOperation(descriptor.rgbBlendOperation);
    auto alphaBlendOperation = UtilsGL::toGLBlendOperation(descriptor.alphaBlendOperation);
    auto sourceRGBBlendFactor = UtilsGL::toGLBlendFactor(descriptor.sourceRGBBlendFactor);
    auto destinationRGBBlendFactor = UtilsGL::toGLBlendFactor(descriptor.destinationRGBBlendFactor);
    auto sourceAlphaBlendFactor = UtilsGL::toGLBlendFactor(descriptor.sourceAlphaBlendFactor);
    auto destinationAlphaBlendFactor = UtilsGL::toGLBlendFactor(descriptor.destinationAlphaBlendFactor);
    auto writeMaskRed = (uint32_t)descriptor.writeMask & (uint32_t)ColorWriteMask::RED;
    auto writeMaskGreen = (uint32_t)descriptor.writeMask & (uint32_t)ColorWriteMask::GREEN;
    auto writeMaskBlue = (uint32_t)descriptor.writeMask & (uint32_t)ColorWriteMask::BLUE;
    auto writeMaskAlpha = (uint32_t)descriptor.writeMask & (uint32_t)ColorWriteMask::ALPHA;

    if (blendEnabled) {
        glEnable(GL_BLEND);
        glBlendEquationSeparate(rgbBlendOperation, alphaBlendOperation);
        glBlendFuncSeparate(sourceRGBBlendFactor,
                            destinationRGBBlendFactor,
                            sourceAlphaBlendFactor,
                            destinationAlphaBlendFactor);
    } else {
        glDisable(GL_BLEND);
    }
    glColorMask(writeMaskRed, writeMaskGreen, writeMaskBlue, writeMaskAlpha);
}

RenderPipelineGL::~RenderPipelineGL() {
    SAFE_RELEASE(program_gl);
}

BACKEND_END
