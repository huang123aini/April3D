//
//  RenderCommand.hpp
//  april3d
//
//  Created by 黄世平 on 2022/4/4.
//

#ifndef RenderCommand_hpp
#define RenderCommand_hpp

#include <stdio.h>

#include "platform/AprilPlatformMacros.h"
#include "base/aprilTypes.h"
#include "renderer/PipelineDescriptor.h"

APRIL_BEGIN

/**
 *绘制Command基类、Renderer知道如何绘制它们
 */

class RenderCommand {
public:
    enum class Type {
        UNKNOWN_COMMAND,
        /**
         *绘制长方形
         */
        QUAD_COMMAND,
        
        CUSTOM_COMMAND,
        /**
         *在树的层次结构中对命令进行分组
         */
        GROUP_COMMAND,
        /**
         *绘制3D元素
         */
        MESH_COMMAND,
        /**
         *绘制三角形
         */
        TRIANGLES_COMMAND,
        /**
         *回调绘制
         */
        CALLBACK_COMMAND,
        /**
         *截屏绘制
         */
        CAPTURE_SCREEN_COMMAND
    };

    void init(float globalZOrder, const Mat4& modelViewTransform, unsigned int flags);
    
    float getGlobalOrder() const { return global_order; }

    Type getType() const { return type_; }
    
    bool isTransparent() const { return is_transparent; }
    
    void setTransparent(bool isTransparent) { is_transparent = isTransparent; }
    /**
     *获取跳过批处理状态，如果一个呈现是跳过批处理，它将被强制分开呈现
     */
    bool isSkipBatching() const { return skip_batching; }
    
    void setSkipBatching(bool value) { skip_batching = value; }
   
    bool is3D() const { return is_3D; }
    
    void set3D(bool value) { is_3D = value; }
  
    float getDepth() const { return depth_; }
   
    inline PipelineDescriptor& getPipelineDescriptor() { return pipeline_descriptor; }

    const Mat4 & getMV() const { return mv_matrix; }

protected:
    RenderCommand();
    
    virtual ~RenderCommand();
    
    void printID();


    Type type_ = RenderCommand::Type::UNKNOWN_COMMAND;

    float global_order = 0.f;
    
    /**
     *透明标志
     */
    bool is_transparent = true;
    
    /**
     *如果材质ID相同，QuadCommand和TrianglesCommand可以自动批处理,一个命令是跳过批处理，它将被迫在一个单独的函数调用中绘制，并中断批处理
     */
    bool skip_batching = false;
    
    /**
     *是否绘制在3d通道
     */
    bool is_3D = false;
    
    /**
     *从模型视图矩阵的深度.
     */
    float depth_ = 0.f;

    Mat4 mv_matrix;

    PipelineDescriptor pipeline_descriptor;
};

APRIL_END

#endif /* RenderCommand_hpp */
