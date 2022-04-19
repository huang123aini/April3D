//
//  Device.hpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/2.
//

#ifndef Device_hpp
#define Device_hpp

#include <stdio.h>
#include <string>

#include "Macros.h"
#include "Types.h"
#include "RenderPipelineDescriptor.h"
#include "RenderPassDescriptor.h"
#include "Texture.h"
#include "DepthStencilState.h"
#include "ProgramCache.h"
#include "ShaderCache.h"
#include "DeviceInfo.h"

#include "base/Ref.h"

BACKEND_BEGIN

class CommandBuffer;
class Buffer;
class ShaderModule;
class RenderPipeline;
class RenderPass;

/**
 * New or create resources from Device.
 */
class Device : public april3d::Ref {
public:
    friend class ProgramCache;
    friend class ShaderCache;
  
    static Device* getInstance();
    virtual ~Device() = default;
    
    /**
     *生成CommandBuffer对象，不自动释放
     */
    virtual CommandBuffer* newCommandBuffer() = 0;

    /**
     *@brief 创建一个Buffer
     */
    virtual Buffer* newBuffer(size_t size, BufferType type, BufferUsage usage) = 0;

    /**
     *@brief生成纹理对象，不自动释放
     */
    virtual TextureBackend* newTexture(const TextureDescriptor& descriptor) = 0;

    /**
     *生成可以自动释放的DepthStencilState对象
     */
    virtual DepthStencilState* createDepthStencilState(const DepthStencilDescriptor& descriptor) = 0;

    /**
     *@brief生成渲染管线,不自动释放
     */
    virtual RenderPipeline* newRenderPipeline() = 0;

    virtual void setFrameBufferOnly(bool frameBufferOnly) = 0;

    virtual Program* newProgram(const std::string& vertexShader, const std::string& fragmentShader) = 0;

    inline DeviceInfo* getDeviceInfo() const { return device_info; }

protected:
    virtual ShaderModule* newShaderModule(ShaderStage stage, const std::string& source) = 0;
    DeviceInfo* device_info = nullptr;
private:
    static Device* shared_instance;
};

BACKEND_END


#endif /* Device_hpp */
