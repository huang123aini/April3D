//
//  DeviceGL.cpp
//  april3d
//
//  Created by 黄世平 on 2022/4/3.
//

#include "DeviceGL.h"
#include "RenderPipelineGL.h"
#include "BufferGL.h"
#include "ShaderModuleGL.h"
#include "CommandBufferGL.h"
#include "TextureGL.h"
#include "DepthStencilStateGL.h"
#include "ProgramGL.h"
#include "DeviceInfoGL.h"

BACKEND_BEGIN

Device* Device::getInstance() {
    if (!shared_instance) {
        shared_instance = new (std::nothrow) DeviceGL();
    }
    return shared_instance;
}

DeviceGL::DeviceGL() {
    device_info = new (std::nothrow) DeviceInfoGL();
    if(!device_info || device_info->init() == false) {
        delete device_info;
        device_info = nullptr;
    }
}

DeviceGL::~DeviceGL() {
    ProgramCache::destroyInstance();
    delete device_info;
    device_info = nullptr;
}

CommandBuffer* DeviceGL::newCommandBuffer() {
    return new (std::nothrow) CommandBufferGL();
}

Buffer* DeviceGL::newBuffer(std::size_t size, BufferType type, BufferUsage usage) {
    return new (std::nothrow) BufferGL(size, type, usage);
}

TextureBackend* DeviceGL::newTexture(const TextureDescriptor& descriptor) {
    switch (descriptor.texture_type) {
    case TextureType::TEXTURE_2D:
        return new (std::nothrow) Texture2DGL(descriptor);
    case TextureType::TEXTURE_CUBE:
        return new (std::nothrow) TextureCubeGL(descriptor);
    default:
        return nullptr;
    }
}

ShaderModule* DeviceGL::newShaderModule(ShaderStage stage, const std::string& source) {
    return new (std::nothrow) ShaderModuleGL(stage, source);
}

DepthStencilState* DeviceGL::createDepthStencilState(const DepthStencilDescriptor& descriptor) {
    auto ret = new (std::nothrow) DepthStencilStateGL(descriptor);
    if (ret) {
        ret->autorelease();
    }
    return ret;
}

RenderPipeline* DeviceGL::newRenderPipeline() {
    return new (std::nothrow) RenderPipelineGL();
}

Program* DeviceGL::newProgram(const std::string& vertexShader, const std::string& fragmentShader) {
    return new (std::nothrow) ProgramGL(vertexShader, fragmentShader);
}

BACKEND_END
