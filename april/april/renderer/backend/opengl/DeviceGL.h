//
//  DeviceGL.hpp
//  april3d
//
//  Created by 黄世平 on 2022/4/3.
//

#ifndef DeviceGL_hpp
#define DeviceGL_hpp

#include <stdio.h>

#include "../Device.h"

BACKEND_BEGIN

class DeviceGL : public Device
{
public:
    DeviceGL();
    ~DeviceGL();

    virtual CommandBuffer* newCommandBuffer() override;

    virtual Buffer* newBuffer(std::size_t size, BufferType type, BufferUsage usage) override;

    virtual TextureBackend* newTexture(const TextureDescriptor& descriptor) override;

    virtual DepthStencilState* createDepthStencilState(const DepthStencilDescriptor& descriptor) override;

    virtual RenderPipeline* newRenderPipeline() override;

    /**
     * Design for metal.
     */
    virtual void setFrameBufferOnly(bool frameBufferOnly) override {}

    virtual Program* newProgram(const std::string& vertexShader, const std::string& fragmentShader) override;

protected:
 
    virtual ShaderModule* newShaderModule(ShaderStage stage, const std::string& source) override;

};

BACKEND_END


#endif /* DeviceGL_hpp */
