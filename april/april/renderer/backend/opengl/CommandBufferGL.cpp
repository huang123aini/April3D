//
//  CommandBufferGL.cpp
//  april3d
//
//  Created by 黄世平 on 2022/4/3.
//

#include <algorithm>

#include "CommandBufferGL.h"
#include "BufferGL.h"
#include "RenderPipelineGL.h"
#include "TextureGL.h"
#include "DepthStencilStateGL.h"
#include "ProgramGL.h"
#include "base/aprilMacros.h"
#include "platform/AprilGL.h"
#include "UtilsGL.h"

#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>

BACKEND_BEGIN

namespace {

GLuint getHandler(TextureBackend *texture) {
    switch (texture->getTextureType()) {
        case TextureType::TEXTURE_2D:
        {
            return static_cast<Texture2DGL*>(texture)->getHandler();
        }
        case TextureType::TEXTURE_CUBE:
        {
            return static_cast<TextureCubeGL*>(texture)->getHandler();
        }
        default:
            assert(false);
            return 0;
    }
}

void applyTexture(TextureBackend* texture, int slot) {
    switch (texture->getTextureType()) {
        case TextureType::TEXTURE_2D:
        {
            static_cast<Texture2DGL*>(texture)->apply(slot);
            break;
        }
        case TextureType::TEXTURE_CUBE:
        {
            static_cast<TextureCubeGL*>(texture)->apply(slot);
            break;
        }
        default:
            assert(false);
            return ;
    }
}
}

CommandBufferGL::CommandBufferGL() {
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &default_fbo);
    
#if ENABLE_CACHE_TEXTURE_DATA
    back_to_foreground_listener = EventListenerCustom::create(EVENT_RENDERER_RECREATED, [this](EventCustom*){
        if(generated_fbo)
            glGenFramebuffers(1, &generated_fbo); //recreate framebuffer
    });
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_backToForegroundListener, -1);
#endif
}

CommandBufferGL::~CommandBufferGL() {
    glDeleteFramebuffers(1, &generated_fbo);
    SAFE_RELEASE_NULL(render_pipeline);
    cleanResources();

#if ENABLE_CACHE_TEXTURE_DATA
    Director::getInstance()->getEventDispatcher()->removeEventListener(_backToForegroundListener);
#endif
}

void CommandBufferGL::beginFrame() {}

void CommandBufferGL::beginRenderPass(const RenderPassDescriptor& descirptor) {
    applyRenderPassDescriptor(descirptor);
}

void CommandBufferGL::applyRenderPassDescriptor(const RenderPassDescriptor& descirptor) {
    bool useColorAttachmentExternal = descirptor.need_color_attachment && descirptor.color_attachments_texture[0];
    bool useDepthAttachmentExternal = descirptor.depth_test_enabled && descirptor.depth_attachment_texture;
    bool useStencilAttachmentExternal = descirptor.stencil_test_enabled && descirptor.stencil_attachment_texture;
    bool useGeneratedFBO = false;
    if (useColorAttachmentExternal || useDepthAttachmentExternal || useStencilAttachmentExternal) {
        if(generated_fbo == 0) {
            glGenFramebuffers(1, &generated_fbo);
        }
        current_fbo = generated_fbo;
        useGeneratedFBO = true;
    } else {
        current_fbo = default_fbo;
    }
    /**
     *绑定FBO
     */
    glBindFramebuffer(GL_FRAMEBUFFER, current_fbo);
    
    /**
     *是否绑定深度缓冲区
     */
    if (useDepthAttachmentExternal) {
        glFramebufferTexture2D(GL_FRAMEBUFFER,
                               GL_DEPTH_ATTACHMENT,
                               GL_TEXTURE_2D,
                               getHandler(descirptor.depth_attachment_texture),
                               0);
        CHECK_GL_ERROR_DEBUG();

        generated_fbo_bind_depth = true;
    } else {
        if (generated_fbo_bind_depth && useGeneratedFBO) {
            glFramebufferTexture2D(GL_FRAMEBUFFER,
                                   GL_DEPTH_ATTACHMENT,
                                   GL_TEXTURE_2D,
                                   0,
                                   0);
            CHECK_GL_ERROR_DEBUG();
            generated_fbo_bind_depth = false;
        }
    }
       
    /**
     *是否绑定纹理缓冲区
     */
    if (useStencilAttachmentExternal) {
        glFramebufferTexture2D(GL_FRAMEBUFFER,
                               GL_STENCIL_ATTACHMENT,
                               GL_TEXTURE_2D,
                               getHandler(descirptor.depth_attachment_texture),
                               0);
        CHECK_GL_ERROR_DEBUG();
        generated_fbo_bind_stencil = true;
    } else {
        if (generated_fbo_bind_stencil && useGeneratedFBO) {
            glFramebufferTexture2D(GL_FRAMEBUFFER,
                                   GL_STENCIL_ATTACHMENT,
                                   GL_TEXTURE_2D,
                                   0,
                                   0);
            CHECK_GL_ERROR_DEBUG();
            generated_fbo_bind_stencil = false;
        }
    }
    
    /**
     *绑定颜色缓冲区
     */
    if (descirptor.need_color_attachment) {
        int i = 0;
        for (const auto& texture : descirptor.color_attachments_texture) {
            if (texture) {
                // TODO: support texture cube
                glFramebufferTexture2D(GL_FRAMEBUFFER,
                                       GL_COLOR_ATTACHMENT0 + i,
                                       GL_TEXTURE_2D,
                                       getHandler(texture),
                                       0);
            }
            CHECK_GL_ERROR_DEBUG();
            ++i;
        }
        if (useGeneratedFBO) {
            generated_fbo_bind_color = true;
        }
            
    } else {
        if (generated_fbo_bind_color && useGeneratedFBO) {
           // FIXME: Now only support attaching to attachment 0.
           glFramebufferTexture2D(GL_FRAMEBUFFER,
                                  GL_COLOR_ATTACHMENT0,
                                  GL_TEXTURE_2D,
                                  0,
                                  0);
            generated_fbo_bind_color = false;
        }

#if APRIL_OS == APRIL_OS_WIN32 || APRIL_OS == APRIL_OS_LINUX
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
#endif
    }
    CHECK_GL_ERROR_DEBUG();
    
    // set clear color, depth and stencil
    GLbitfield mask = 0;
    if (descirptor.need_clear_color) {
        mask |= GL_COLOR_BUFFER_BIT;
        const auto& clearColor = descirptor.clear_color_value;
        glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
    }
    
    CHECK_GL_ERROR_DEBUG();

    GLboolean oldDepthWrite = GL_FALSE;
    GLboolean oldDepthTest = GL_FALSE;
    GLfloat oldDepthClearValue = 0.f;
    GLint oldDepthFunc = GL_LESS;
    
    /**
     *设置深度参数
     */
    if (descirptor.need_clear_depth) {
        glGetBooleanv(GL_DEPTH_WRITEMASK, &oldDepthWrite);
        glGetBooleanv(GL_DEPTH_TEST, &oldDepthTest);
        glGetFloatv(GL_DEPTH_CLEAR_VALUE, &oldDepthClearValue);
        glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFunc);
        
        mask |= GL_DEPTH_BUFFER_BIT;
        glClearDepth(descirptor.clear_depth_value);
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_ALWAYS);
    }
    
    CHECK_GL_ERROR_DEBUG();
    
    if (descirptor.need_clear_stencil) {
        mask |= GL_STENCIL_BUFFER_BIT;
        glClearStencil(descirptor.clear_stencil_value);
    }

    if(mask) {
        /**
         *缓冲区清理
         */
        glClear(mask);
    }
    
    CHECK_GL_ERROR_DEBUG();
    
    // restore depth test
    if (descirptor.need_clear_depth) {
        if (!oldDepthTest) {
            glDisable(GL_DEPTH_TEST);
        }
        glDepthMask(oldDepthWrite);
        glDepthFunc(oldDepthFunc);
        glClearDepth(oldDepthClearValue);
    }
    
    CHECK_GL_ERROR_DEBUG();
}

void CommandBufferGL::setRenderPipeline(RenderPipeline* renderPipeline) {
    assert(renderPipeline != nullptr);
    if (renderPipeline == nullptr)
        return;
    
    RenderPipelineGL* rp = static_cast<RenderPipelineGL*>(renderPipeline);
    rp->retain();
    SAFE_RELEASE(render_pipeline);
    render_pipeline = rp;
}

void CommandBufferGL::setViewport(int x, int y, unsigned int w, unsigned int h) {
    glViewport(x, y, w, h);
    view_port.x = x;
    view_port.y = y;
    view_port.w = w;
    view_port.h = h;
}

void CommandBufferGL::setCullMode(CullMode mode) {
    cull_mode = mode;
}

void CommandBufferGL::setWinding(Winding winding) {
    glFrontFace(UtilsGL::toGLFrontFace(winding));
}

void CommandBufferGL::setIndexBuffer(Buffer* buffer) {
    assert(buffer != nullptr);
    if (buffer == nullptr) {
        return;
    }
    buffer->retain();
    SAFE_RELEASE(index_buffer);
    index_buffer = static_cast<BufferGL*>(buffer);
}

void CommandBufferGL::setVertexBuffer(Buffer* buffer) {
    assert(buffer != nullptr);
    if (buffer == nullptr || vertex_buffer == buffer) {
        return;
    }
    buffer->retain();
    vertex_buffer = static_cast<BufferGL*>(buffer);
}

void CommandBufferGL::setProgramState(ProgramState* programState) {
    SAFE_RETAIN(programState);
    SAFE_RELEASE(program_state);
    program_state = programState;
}

void CommandBufferGL::drawArrays(PrimitiveType primitiveType, std::size_t start,  std::size_t count) {
    prepareDrawing();
    glDrawArrays(UtilsGL::toGLPrimitiveType(primitiveType), start, count);
    
    cleanResources();
}

void CommandBufferGL::drawElements(PrimitiveType primitiveType, IndexFormat indexType, std::size_t count, std::size_t offset) {
    prepareDrawing();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer->getHandler());
    glDrawElements(UtilsGL::toGLPrimitiveType(primitiveType), count, UtilsGL::toGLIndexType(indexType), (GLvoid*)offset);
    CHECK_GL_ERROR_DEBUG();
    cleanResources();
}

void CommandBufferGL::endRenderPass() {}

void CommandBufferGL::endFrame() {}

void CommandBufferGL::setDepthStencilState(DepthStencilState* depthStencilState) {
    if (depthStencilState) {
        depth_stencil_state_gl = static_cast<DepthStencilStateGL*>(depthStencilState);
    } else {
        depth_stencil_state_gl = nullptr;
    }
}

void CommandBufferGL::prepareDrawing() const
{
    const auto& program = render_pipeline->getProgram();
    glUseProgram(program->getHandler());
    
    bindVertexBuffer(program);
    setUniforms(program);

    // Set depth/stencil state.
    if (depth_stencil_state_gl) {
        depth_stencil_state_gl->apply(stencil_reference_value_front, stencil_reference_value_back);
    }
        
    else
        DepthStencilStateGL::reset();
    
    // Set cull mode.
    if (CullMode::NONE == cull_mode) {
        glDisable(GL_CULL_FACE);
    } else {
        glEnable(GL_CULL_FACE);
        glCullFace(UtilsGL::toGLCullMode(cull_mode));
    }
}

void CommandBufferGL::bindVertexBuffer(ProgramGL *program) const {
    // Bind vertex buffers and set the attributes.
    auto vertexLayout = program_state->getVertexLayout();
    
    if (!vertexLayout->isValid()) {
        return;
    }

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer->getHandler());

    const auto& attributes = vertexLayout->getAttributes();
    for (const auto& attributeInfo : attributes) {
        const auto& attribute = attributeInfo.second;
        glEnableVertexAttribArray(attribute.index_);
        glVertexAttribPointer(attribute.index_,
            UtilsGL::getGLAttributeSize(attribute.format_),
            UtilsGL::toGLAttributeType(attribute.format_),
            attribute.need_to_be_normallized,
            vertexLayout->getStride(),
            (GLvoid*)attribute.offset_);
    }
}

void CommandBufferGL::setUniforms(ProgramGL* program) const
{
    if (program_state)
    {
        auto& callbacks = program_state->getCallbackUniforms();
        auto& uniformInfos = program_state->getProgram()->getAllActiveUniformInfo(ShaderStage::VERTEX);
        std::size_t bufferSize = 0;
        char* buffer = nullptr;
        program_state->getVertexUniformBuffer(&buffer, bufferSize);

        for (auto &cb : callbacks)
        {
            cb.second(program_state, cb.first);
        }

        int i = 0;
        for(auto& iter : uniformInfos)
        {
            auto& uniformInfo = iter.second;
            if(uniformInfo.size_ <= 0)
                continue;

            int elementCount = uniformInfo.count_;
            setUniform(uniformInfo.is_array,
                uniformInfo.location_,
                elementCount,
                uniformInfo.type_,
                (void*)(buffer + uniformInfo.buffer_offset));
        }
        
        const auto& textureInfo = program_state->getVertexTextureInfos();
        for(const auto& iter : textureInfo) {
            const auto& textures = iter.second.textures_;
            const auto& slot = iter.second.slot_;
            auto location = iter.first;
            int i = 0;
            for (const auto& texture: textures) {
                applyTexture(texture, slot[i]);
                ++i;
            }
            
            auto arrayCount = slot.size();
            if (arrayCount > 1) {
                glUniform1iv(location, (uint32_t)arrayCount, (GLint*)slot.data());
            } else {
                glUniform1i(location, slot[0]);
            }
        }
    }
}

#define DEF_TO_INT(pointer, index)     (*((GLint*)(pointer) + index))
#define DEF_TO_FLOAT(pointer, index)   (*((GLfloat*)(pointer) + index))
void CommandBufferGL::setUniform(bool isArray, GLuint location, unsigned int size, GLenum uniformType, void* data) const {
    GLsizei count = size;
    switch (uniformType)
    {
        case GL_INT:
        case GL_BOOL:
        case GL_SAMPLER_2D:
        case GL_SAMPLER_CUBE:
            if (isArray)
                glUniform1iv(location, count, (GLint*)data);
            else
                glUniform1i(location, DEF_TO_INT(data, 0));
            break;
        case GL_INT_VEC2:
        case GL_BOOL_VEC2:
            if (isArray)
                glUniform2iv(location, count, (GLint*)data);
            else
                glUniform2i(location, DEF_TO_INT(data, 0), DEF_TO_INT(data, 1));
            break;
        case GL_INT_VEC3:
        case GL_BOOL_VEC3:
            if (isArray)
                glUniform3iv(location, count, (GLint*)data);
            else
                glUniform3i(location,
                            DEF_TO_INT(data, 0),
                            DEF_TO_INT(data, 1),
                            DEF_TO_INT(data, 2));
            break;
        case GL_INT_VEC4:
        case GL_BOOL_VEC4:
            if (isArray)
                glUniform4iv(location, count, (GLint*)data);
            else
                glUniform4i(location,
                            DEF_TO_INT(data, 0),
                            DEF_TO_INT(data, 1),
                            DEF_TO_INT(data, 2),
                            DEF_TO_INT(data, 4));
            break;
        case GL_FLOAT:
            if (isArray)
                glUniform1fv(location, count, (GLfloat*)data);
            else
                glUniform1f(location, DEF_TO_FLOAT(data, 0));
            break;
        case GL_FLOAT_VEC2:
            if (isArray)
                glUniform2fv(location, count, (GLfloat*)data);
            else
                glUniform2f(location, DEF_TO_FLOAT(data, 0), DEF_TO_FLOAT(data, 1));
            break;
        case GL_FLOAT_VEC3:
            if (isArray)
                glUniform3fv(location, count, (GLfloat*)data);
            else
                glUniform3f(location,
                            DEF_TO_FLOAT(data, 0),
                            DEF_TO_FLOAT(data, 1),
                            DEF_TO_FLOAT(data, 2));
            break;
        case GL_FLOAT_VEC4:
            if (isArray)
                glUniform4fv(location, count, (GLfloat*)data);
            else
                glUniform4f(location,
                            DEF_TO_FLOAT(data, 0),
                            DEF_TO_FLOAT(data, 1),
                            DEF_TO_FLOAT(data, 2),
                            DEF_TO_FLOAT(data, 3));
            break;
        case GL_FLOAT_MAT2:
            glUniformMatrix2fv(location, count, GL_FALSE, (GLfloat*)data);
            break;
        case GL_FLOAT_MAT3:
            glUniformMatrix3fv(location, count, GL_FALSE, (GLfloat*)data);
            break;
        case GL_FLOAT_MAT4:
            glUniformMatrix4fv(location, count, GL_FALSE, (GLfloat*)data);
            break;
        break;
        
        default:
            printf("invalidate Uniform data type \n");
        break;
    }
}

void CommandBufferGL::cleanResources() {
    SAFE_RELEASE_NULL(index_buffer);
    SAFE_RELEASE_NULL(program_state);
    SAFE_RELEASE_NULL(vertex_buffer);
}

void CommandBufferGL::setLineWidth(float lineWidth) {
    if(lineWidth > 0.0f) {
        glLineWidth(lineWidth);
    } else {
        glLineWidth(1.0f);
    }
}


void CommandBufferGL::setScissorRect(bool isEnabled, float x, float y, float width, float height) {
    if(isEnabled) {
        glEnable(GL_SCISSOR_TEST);
        glScissor(x, y, width, height);
    } else {
        glDisable(GL_SCISSOR_TEST);
    }
}

void CommandBufferGL::captureScreen(std::function<void(const unsigned char*, int, int)> callback) {
    int bufferSize = view_port.w * view_port.h *4;
    std::shared_ptr<GLubyte> buffer(new GLubyte[bufferSize], [](GLubyte* p){ SAFE_DELETE_ARRAY(p); });
    memset(buffer.get(), 0, bufferSize);
    if (!buffer) {
        callback(nullptr, 0, 0);
        return;
    }
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, view_port.w, view_port.h, GL_RGBA, GL_UNSIGNED_BYTE, buffer.get());

    std::shared_ptr<GLubyte> flippedBuffer(new GLubyte[bufferSize], [](GLubyte* p) { SAFE_DELETE_ARRAY(p); });
    memset(flippedBuffer.get(), 0, bufferSize);
    if (!flippedBuffer) {
        callback(nullptr, 0, 0);
        return;
    }
    for (int row = 0; row < view_port.h; ++row) {
        memcpy(flippedBuffer.get() + (view_port.h - row - 1) * view_port.w * 4, buffer.get() + row * view_port.w * 4, view_port.w * 4);
    }
    callback(flippedBuffer.get(), view_port.w, view_port.h);
}

BACKEND_END
