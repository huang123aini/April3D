//
//  Renderer.hpp
//  april3d
//
//  Created by 黄世平 on 2022/4/4.
//

#ifndef Renderer_hpp
#define Renderer_hpp

#include <stdio.h>
#include <vector>
#include <stack>
#include <array>
#include <deque>

#include "platform/AprilPlatformMacros.h"
#include "renderer/RenderCommand.h"
#include "renderer/backend/Types.h"

APRIL_BEGIN

using CullMode = backend::CullMode;

using Winding = backend::Winding;

namespace backend {
    class Buffer;
    class CommandBuffer;
    class RenderPipeline;
    class RenderPass;
    struct RenderPipelineDescriptor;
}

class TrianglesCommand;
class MeshCommand;
class GroupCommand;
class CallbackCommand;
struct PipelineDescriptor;
class Texture2D;

/**
 渲染队列
 Class that knows how to sort `RenderCommand` objects.
 Since the commands that have `z == 0` are "pushed back" in
 the correct order, the only `RenderCommand` objects that need to be sorted,
 are the ones that have `z < 0` and `z > 0`.
*/
class RenderQueue {
public:
    /**
    RenderCommand will be divided into Queue Groups.
    */
    enum QUEUE_GROUP {
        GLOBALZ_NEG = 0,
        OPAQUE_3D = 1,
        TRANSPARENT_3D = 2,
        GLOBALZ_ZERO = 3,
        GLOBALZ_POS = 4,
        QUEUE_COUNT = 5,
    };

public:
    RenderQueue();
    
    void push_back(RenderCommand* command);

    ssize_t size() const;
    
    void sort();
    
    RenderCommand* operator[](ssize_t index) const;
    
    void clear();
    
    void realloc(size_t reserveSize);
    
    std::vector<RenderCommand*>& getSubQueue(QUEUE_GROUP group) { return _commands[group]; }
    
    ssize_t getSubQueueSize(QUEUE_GROUP group) const { return _commands[group].size(); }
    
protected:
    std::vector<RenderCommand*> commands_[QUEUE_COUNT];
    
    bool is_cull_enabled;
    
    bool is_depth_enabled;
 
    bool is_depth_write;
};

class GroupCommandManager;

class Renderer {
public:
    
    static const int VBO_SIZE = 65536;
    
    static const int INDEX_VBO_SIZE = VBO_SIZE * 6 / 4;
    
    static const int BATCH_TRIAGCOMMAND_RESERVED_SIZE = 64;
    
    static const int MATERIAL_ID_DO_NOT_BATCH = 0;

    Renderer();

    ~Renderer();

    void init();

    void addCommand(RenderCommand* command);

    void addCommand(RenderCommand* command, int renderQueueID);

    void pushGroup(int renderQueueID);

    void popGroup();

    int createRenderQueue();

    void render();

    void clean();

    ssize_t getDrawnBatches() const { return drawn_batches; }

    void addDrawnBatches(ssize_t number) { drawn_batches += number; };

    ssize_t getDrawnVertices() const { return drawn_vertices; }

    void addDrawnVertices(ssize_t number) { drawn_vertices += number; };

    void clearDrawStats() { drawn_batches = drawn_vertices = 0; }

    void setRenderTarget(RenderTargetFlag flags, Texture2D* colorAttachment, Texture2D* depthAttachment, Texture2D* stencilAttachment);

    void clear(ClearFlag flags, const Color4F& color, float depth, unsigned int stencil, float globalOrder);

    Texture2D* getColorAttachment() const;

    Texture2D* getDepthAttachment() const;

    Texture2D* getStencilAttachment() const;

    const Color4F& getClearColor() const;

    float getClearDepth() const;

    unsigned int getClearStencil() const;

    ClearFlag getClearFlag() const;

    RenderTargetFlag getRenderTargetFlag() const;

    void setDepthTest(bool value);

    void setDepthWrite(bool value);

    void setDepthCompareFunction(backend::CompareFunction func);

    bool getDepthTest() const;

    bool getDepthWrite() const;

    backend::CompareFunction getDepthCompareFunction() const;

    void setStencilTest(bool value);

    void setStencilCompareFunction(backend::CompareFunction func, unsigned int ref, unsigned int readMask);

    void setStencilOperation(backend::StencilOperation stencilFailureOp,
                             backend::StencilOperation depthFailureOp,
                             backend::StencilOperation stencilDepthPassOp);

    void setStencilWriteMask(unsigned int mask);

    bool getStencilTest() const;

    backend::StencilOperation getStencilFailureOperation() const;

    backend::StencilOperation getStencilPassDepthFailureOperation() const;

    backend::StencilOperation getStencilDepthPassOperation() const;

    backend::CompareFunction getStencilCompareFunction() const;
    
    unsigned int getStencilReadMask() const;

    unsigned int getStencilWriteMask() const;
    
    unsigned int getStencilReferenceValue() const;

    void setCullMode(CullMode mode) { cull_mode = mode; }

    CullMode getCullMode() const { return cull_mode; }

    void setWinding(Winding winding) { winding_ = winding; }

    Winding getWinding() const { return winding_; }

    void setViewPort(int x, int y, unsigned int w, unsigned int h);

    const Viewport& getViewport() const { return view_port; }

    void setScissorTest(bool enabled);

    void setScissorRect(float x, float y, float width, float height);
    
    bool getScissorTest() const;
    
    const ScissorRect& getScissorRect() const;

    bool checkVisibility(const Mat4& transform, const Size& size);
    
protected:
    friend class Director;
    
    friend class GroupCommand;

    class TriangleCommandBufferManager {
    public:
        ~TriangleCommandBufferManager();

        void init();

        void putbackAllBuffers();

        void prepareNextBuffer();

        backend::Buffer* getVertexBuffer() const;
        backend::Buffer* getIndexBuffer() const;

    private:
        void createBuffer();
        int current_buffer_index = 0;
        std::vector<backend::Buffer*> vertex_buffer_pool;
        std::vector<backend::Buffer*> index_buffer_pool;
    };

    inline GroupCommandManager * getGroupCommandManager() const { return group_command_manager; }
    
    void drawBatchedTriangles();
    
    void drawCustomCommand(RenderCommand* command);

    void drawMeshCommand(RenderCommand* command);
    
    void captureScreen(RenderCommand* command);

    void beginFrame();

    void endFrame();

    ///Draw the previews queued triangles and flush previous context
    void flush();
    
    void flush2D();
    
    void flush3D();

    void flushTriangles();

    void processRenderCommand(RenderCommand* command);
    
    void processGroupCommand(GroupCommand*);
    
    void visitRenderQueue(RenderQueue& queue);
    
    void doVisitRenderQueue(const std::vector<RenderCommand*>&);

    void fillVerticesAndIndices(const TrianglesCommand* cmd, unsigned int vertexBufferOffset);
    
    void beginRenderPass(RenderCommand*); /// Begin a render pass.
    
    void setRenderPipeline(const PipelineDescriptor&, const backend::RenderPassDescriptor&);

    void pushStateBlock();

    void popStateBlock();

    backend::RenderPipeline* _renderPipeline = nullptr;

    Viewport view_port;
    
    CullMode cull_mode  = CullMode::NONE;
    
    /**
     *default front face is CCW in GL
     */
    Winding winding_    = Winding::COUNTER_CLOCK_WISE;

    std::stack<int> _commandGroupStack;
    
    std::vector<RenderQueue> _renderGroups;

    std::vector<TrianglesCommand*> _queuedTriangleCommands;

    //for TrianglesCommand
    V3F_C4B_T2F _verts[VBO_SIZE];
    unsigned short _indices[INDEX_VBO_SIZE];
    backend::Buffer* _vertexBuffer = nullptr;
    backend::Buffer* _indexBuffer = nullptr;
    TriangleCommandBufferManager _triangleCommandBufferManager;
    
    backend::CommandBuffer* _commandBuffer = nullptr;
    
    backend::RenderPassDescriptor _renderPassDescriptor;
    
    backend::DepthStencilDescriptor _depthStencilDescriptor;

    // Internal structure that has the information for the batches
    struct TriBatchToDraw
    {
        TrianglesCommand* cmd = nullptr;  // needed for the Material
        unsigned int indicesToDraw = 0;
        unsigned int offset = 0;
    };
    // capacity of the array of TriBatches
    int _triBatchesToDrawCapacity = 500;
    // the TriBatches
    TriBatchToDraw* _triBatchesToDraw = nullptr;

    unsigned int _queuedTotalVertexCount = 0;
    unsigned int _queuedTotalIndexCount = 0;
    unsigned int _queuedVertexCount = 0;
    unsigned int _queuedIndexCount = 0;
    unsigned int _filledIndex = 0;
    unsigned int _filledVertex = 0;

    // stats
    unsigned int drawn_batches = 0;
    unsigned int drawn_vertices = 0;
    bool is_rendering = false;
    bool is_depth_test_for_2D = false;
        
    GroupCommandManager* group_command_manager = nullptr;

    unsigned int stencil_ref = 0;

    // weak reference
    Texture2D* color_attachment = nullptr;
    
    Texture2D* depth_attachment = nullptr;
    
    Texture2D* stencil_attachment = nullptr;
    
    Color4F clear_color = Color4F::BLACK;
    
    ClearFlag clear_flag;
    
    RenderTargetFlag render_target_flag = RenderTargetFlag::COLOR;

    struct ScissorState {
        ScissorRect rect;
        bool isEnabled = false;
    };
    ScissorState _scissorState;
    
    struct StateBlock{
        bool depthTest = false;
        bool depthWrite = false;
        backend::CullMode  cullMode = backend::CullMode::NONE;
    };

    std::deque<StateBlock> state_block_stack;
};

APRIL_END

#endif /* Renderer_hpp */
