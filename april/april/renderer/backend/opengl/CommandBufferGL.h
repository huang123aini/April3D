//
//  CommandBufferGL.hpp
//  april3d
//
//  Created by 黄世平 on 2022/4/3.
//

#ifndef CommandBufferGL_hpp
#define CommandBufferGL_hpp

#include <stdio.h>
#include "../Macros.h"
#include "../CommandBuffer.h"
#include "platform/AprilGL.h"

#include <vector>

BACKEND_BEGIN

class BufferGL;
class RenderPipelineGL;
class ProgramGL;
class DepthStencilStateGL;

class CommandBufferGL final : public CommandBuffer {
public:
    CommandBufferGL();
    ~CommandBufferGL();
    
    virtual void beginFrame() override;
    virtual void beginRenderPass(const RenderPassDescriptor& descriptor) override;
    virtual void setRenderPipeline(RenderPipeline* renderPipeline) override;
    virtual void setViewport(int x, int y, unsigned int w, unsigned int h) override;
    virtual void setCullMode(CullMode mode) override;
    virtual void setWinding(Winding winding) override;
    virtual void setVertexBuffer(Buffer* buffer) override;
    virtual void setProgramState(ProgramState* programState) override;
    virtual void setIndexBuffer(Buffer* buffer) override;
    
    virtual void drawArrays(PrimitiveType primitiveType, std::size_t start,  std::size_t count) override;
    virtual void drawElements(PrimitiveType primitiveType, IndexFormat indexType, std::size_t count, std::size_t offset) override;
    
    virtual void endRenderPass() override;
    virtual void endFrame() override;
    virtual void setLineWidth(float lineWidth) override;
    virtual void setScissorRect(bool isEnabled, float x, float y, float width, float height) override;
    virtual void setDepthStencilState(DepthStencilState* depthStencilState) override;
    virtual void captureScreen(std::function<void(const unsigned char*, int, int)> callback) override ;
private:
    struct Viewport {
        int x = 0;
        int y = 0;
        unsigned int w = 0;
        unsigned int h = 0;
    };
    
    void prepareDrawing() const;
    void bindVertexBuffer(ProgramGL* program) const;
    void setUniforms(ProgramGL* program) const;
    void setUniform(bool isArray, GLuint location, unsigned int size, GLenum uniformType, void* data) const;
    void cleanResources();
    void applyRenderPassDescriptor(const RenderPassDescriptor& descirptor);

    GLuint generated_fbo = 0;
    bool generated_fbo_bind_color = false;
    bool generated_fbo_bind_depth = false;
    bool generated_fbo_bind_stencil = false;
    
    /**
     *The value gets from glGetIntegerv, so need to use GLint
     */
    GLint default_fbo = 0;
    GLuint current_fbo = 0;
    BufferGL* vertex_buffer;
    ProgramState* program_state = nullptr;
    BufferGL* index_buffer = nullptr;
    RenderPipelineGL* render_pipeline = nullptr;
    CullMode cull_mode = CullMode::NONE;
    DepthStencilStateGL* depth_stencil_state_gl = nullptr;
    Viewport view_port;
    GLboolean alpha_test_enabled = false;
};

BACKEND_END


#endif /* CommandBufferGL_hpp */
