cmake_minimum_required(VERSION 3.10)

set(APRIL_RENDERER_HEADER
    renderer/CallbackCommand.h
    renderer/CustomCommand.h
    renderer/GroupCommand.h
    renderer/Material.h
    renderer/MeshCommand.h
    renderer/Pass.h
    renderer/QuadCommand.h
    renderer/RenderCommand.h
    renderer/RenderCommandPool.h
    renderer/RenderState.h
    renderer/Renderer.h
    renderer/Technique.h
    renderer/Texture2D.h
    #renderer/TextureAtlas.h
    #renderer/TextureCache.h
    #renderer/TextureCube.h
    #renderer/TextureUtils.h
    #renderer/TrianglesCommand.h
    renderer/Shaders.h

########################绘制接口层抽象#####################
    renderer/backend/RHI.h
    renderer/backend/Buffer.h
    renderer/backend/CommandBuffer.h
    renderer/backend/DepthStencilState.h
    renderer/backend/Device.h
    renderer/backend/Program.h
    renderer/backend/ProgramCache.h
    renderer/backend/Macros.h
    renderer/backend/RenderPassDescriptor.h
    renderer/backend/RenderPipeline.h
    renderer/backend/RenderPipelineDescriptor.h
    renderer/PipelineDescriptor.h
    renderer/backend/ShaderModule.h
    renderer/backend/Texture.h
    renderer/backend/Types.h
    renderer/backend/VertexLayout.h
    renderer/backend/ProgramState.h
    renderer/backend/ShaderCache.h
    renderer/backend/DeviceInfo.h
    )

set(APRIL_RENDERER_SRC
    renderer/CallbackCommand.cpp
    renderer/CustomCommand.cpp
    renderer/GroupCommand.cpp
    renderer/Material.cpp
    renderer/MeshCommand.cpp
    renderer/Pass.cpp
    renderer/QuadCommand.cpp
    renderer/RenderCommand.cpp
    renderer/RenderState.cpp
    renderer/Renderer.cpp
    renderer/Technique.cpp
    renderer/Texture2D.cpp
    #renderer/TextureAtlas.cpp
    #renderer/TextureCache.cpp
    #renderer/TextureCube.cpp
    #renderer/TextureUtils.cpp
    #renderer/TrianglesCommand.cpp
    renderer/Shaders.cpp

    renderer/backend/CommandBuffer.cpp
    renderer/backend/DepthStencilState.cpp
    renderer/backend/Device.cpp
    renderer/backend/ShaderModule.cpp
    renderer/backend/Texture.cpp
    renderer/backend/Types.cpp
    renderer/backend/VertexLayout.cpp
    renderer/backend/ProgramCache.cpp
    renderer/backend/Program.cpp
    renderer/backend/ProgramState.cpp
    renderer/backend/ShaderCache.cpp
    renderer/backend/RenderPassDescriptor.cpp
    )

#if(ANDROID OR WINDOWS OR LINUX) 

############TODO:hsp 目前全部使用OpenGL ES 3.2###############
list(APPEND APRIL_RENDERER_HEADER
    renderer/backend/opengl/BufferGL.h
    renderer/backend/opengl/CommandBufferGL.h
    renderer/backend/opengl/DepthStencilStateGL.h
    renderer/backend/opengl/DeviceGL.h
    renderer/backend/opengl/ProgramGL.h
    renderer/backend/opengl/RenderPipelineGL.h
    renderer/backend/opengl/ShaderModuleGL.h
    renderer/backend/opengl/TextureGL.h
    renderer/backend/opengl/UtilsGL.h
    renderer/backend/opengl/DeviceInfoGL.h
)

list(APPEND APRIL_RENDERER_SRC
    renderer/backend/opengl/BufferGL.cpp
    renderer/backend/opengl/CommandBufferGL.cpp
    renderer/backend/opengl/DepthStencilStateGL.cpp
    renderer/backend/opengl/DeviceGL.cpp
    renderer/backend/opengl/ProgramGL.cpp
    renderer/backend/opengl/RenderPipelineGL.cpp
    renderer/backend/opengl/ShaderModuleGL.cpp
    renderer/backend/opengl/TextureGL.cpp
    renderer/backend/opengl/UtilsGL.cpp
    renderer/backend/opengl/DeviceInfoGL.cpp
)

#else()
#list(APPEND APRIL_RENDERER_HEADER
#    renderer/backend/metal/BufferMTL.h
#    renderer/backend/metal/BufferManager.h
#    renderer/backend/metal/CommandBufferMTL.h
#    renderer/backend/metal/DepthStencilStateMTL.h
#    renderer/backend/metal/DeviceMTL.h
#    renderer/backend/metal/RenderPipelineMTL.h
#    renderer/backend/metal/ShaderModuleMTL.h
#    renderer/backend/metal/TextureMTL.h
#    renderer/backend/metal/Utils.h
#    renderer/backend/metal/ProgramMTL.h
#    renderer/backend/metal/DeviceInfoMTL.h
#)
#
#list(APPEND APRIL_RENDERER_SRC
#    renderer/backend/metal/BufferMTL.mm
#    renderer/backend/metal/BufferManager.mm
#    renderer/backend/metal/CommandBufferMTL.mm
#    renderer/backend/metal/DepthStencilStateMTL.mm
#    renderer/backend/metal/DeviceMTL.mm
#    renderer/backend/metal/RenderPipelineMTL.mm
#    renderer/backend/metal/ShaderModuleMTL.mm
#    renderer/backend/metal/TextureMTL.mm
#    renderer/backend/metal/Utils.mm
#    renderer/backend/metal/ProgramMTL.mm
#    renderer/backend/metal/DeviceInfoMTL.mm
#)
#endif()