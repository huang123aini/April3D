//
//  CommandBuffer.hpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/2.
//

#ifndef CommandBuffer_hpp
#define CommandBuffer_hpp

#include <stdio.h>
#include <cstdint>
#include <memory>
#include <vector>

#include "base/Ref.h"
#include "Macros.h"
#include "Types.h"
#include "RenderPassDescriptor.h"
#include "ProgramState.h"
#include "VertexLayout.h"



BACKEND_BEGIN

class RenderPass;
class RenderPipeline;
class Buffer;
class DepthStencilState;


/**
 * @brief存储编码的命令供GPU执行
 *
 */
class CommandBuffer : public april3d::Ref
{
public:
    virtual void beginFrame() = 0;
    
    /**
     *开始渲染，初始颜色，深度和模板附件
     * @param 描述符指定一组呈现目标，包含一个呈现传递的结果。
     */
    virtual void beginRenderPass(const RenderPassDescriptor& descriptor) = 0;
    
    /**
     *设置渲染管线状态
     */
    virtual void setRenderPipeline(RenderPipeline* renderPipeline) = 0;
    
    /**
     *设置视口大小
     */
    virtual void setViewport(int x, int y, unsigned int w, unsigned int h) = 0;

    /**
     * 设置剔除模式
     */
    virtual void setCullMode(CullMode mode) = 0;

    /**
     *设置环绕模式
     */
    virtual void setWinding(Winding winding) = 0;

    /**
     * 在给定的绑定点索引0处为所有的顶点着色器设置一个全局缓冲区
     */
    virtual void setVertexBuffer(Buffer* buffer) = 0;

    /**
     *设置ProgramState
     */
    virtual void setProgramState(ProgramState* programState) = 0;

    /**
     *设置索引buffer
     */
    virtual void setIndexBuffer(Buffer* buffer) = 0;

    /**
     *无索引绘制命令
     */
    virtual void drawArrays(PrimitiveType primitiveType, std::size_t start,  std::size_t count) = 0;

    /**
     *索引绘制命令
    */
    virtual void drawElements(PrimitiveType primitiveType, IndexFormat indexType, std::size_t count, std::size_t offset) = 0;
    
    /**
     *渲染结束
     */
    virtual void endRenderPass() = 0;

    /**
     *提交命令缓冲区
     */
    virtual void endFrame() = 0;
    

    virtual void setLineWidth(float lineWidth) = 0;

    virtual void setScissorRect(bool isEnabled, float x, float y, float width, float height) = 0;

    virtual void setDepthStencilState(DepthStencilState* depthStencilState) = 0;

    virtual void captureScreen(std::function<void(const unsigned char*, int, int)> callback) = 0;
    
    void setStencilReferenceValue(unsigned int value);

    void setStencilReferenceValue(unsigned int frontRef, unsigned int backRef);

protected:
    virtual ~CommandBuffer() = default;
    /**
     *前端模板参考值
     */
    unsigned int stencil_reference_value_front = 0;
    /**
     *背模板参考值
     */
    unsigned int stencil_reference_value_back = 0;
};

BACKEND_END


#endif /* CommandBuffer_hpp */
