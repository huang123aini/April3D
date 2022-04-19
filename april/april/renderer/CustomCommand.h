//
//  CustomCommand.hpp
//  april3d
//
//  Created by 黄世平 on 2022/4/4.
//

#ifndef CustomCommand_hpp
#define CustomCommand_hpp

#include <stdio.h>
#include "renderer/RenderCommand.h"

APRIL_BEGIN

namespace backend {
    class Buffer;
}


class CustomCommand : public RenderCommand {
public:
    enum class DrawType
    {
        ARRAY,
        ELEMENT
    };

    using PrimitiveType = backend::PrimitiveType;
  
    using BufferUsage = backend::BufferUsage;
  
    using IndexFormat = backend::IndexFormat;


    typedef std::function<void()> CallBackFunc;

    CustomCommand();
    
    ~CustomCommand();
    

public:
 
    void init(float globalZOrder, const Mat4& modelViewTransform, unsigned int flags);
    

    void init(float globalZOrder);
 
    void init(float globalZOrder, const BlendFunc& blendFunc);

 
    void createVertexBuffer(std::size_t vertexSize, std::size_t capacity, BufferUsage usage);
  
    void createIndexBuffer(IndexFormat format, std::size_t capacity, BufferUsage usage);

  
    void updateVertexBuffer(void* data, std::size_t length);
 
    void updateIndexBuffer(void* data, std::size_t length);
  
    void updateVertexBuffer(void* data, std::size_t offset, std::size_t length);
   
    void updateIndexBuffer(void* data, std::size_t offset, std::size_t length);

    inline std::size_t getVertexCapacity() const { return vertex_capacity; }
    
    inline std::size_t getIndexCapacity() const { return index_capacity; }

    inline void setDrawType(DrawType drawType) { draw_type = drawType; }
    
    inline DrawType getDrawType() const { return draw_type; }

    inline void setPrimitiveType(PrimitiveType primitiveType) { primitive_type = primitiveType; }
    
    inline PrimitiveType getPrimitiveType() const { return primitive_type; }

    void setVertexBuffer(backend::Buffer* vertexBuffer);

    inline backend::Buffer* getVertexBuffer() const { return vertex_buffer; }

    void setIndexBuffer(backend::Buffer* indexBuffer, IndexFormat indexFormat);
    
    inline backend::Buffer* getIndexBuffer() const { return index_buffer; }

    inline void setVertexDrawInfo(std::size_t start, std::size_t count) { vertex_draw_start = start; vertex_draw_count = count; }

    inline std::size_t getVertexDrawStart() const { return vertex_draw_start; }

    inline std::size_t getVertexDrawCount() const { return vertex_draw_count;}

    inline void setIndexDrawInfo(std::size_t start, std::size_t count) { index_draw_offset = start * index_size; index_draw_count = count; }

    inline std::size_t getIndexDrawOffset() const { return index_draw_offset; }
    
    inline std::size_t getIndexDrawCount() const { return index_draw_count; }
    
    inline void setLineWidth(float lineWidth) { line_width = lineWidth; }
    
    inline float getLineWidth() const { return line_width; }

    inline IndexFormat getIndexFormat() const { return index_format; }

    std::function<void()> func_;

    void setBeforeCallback(const CallBackFunc &before) { before_callback = before; }

    void setAfterCallback(const CallBackFunc &after) { after_callback = after; }

    const CallBackFunc &getBeforeCallback() { return before_callback; }

    const CallBackFunc &getAfterCallback() { return after_callback; }

protected:
    std::size_t computeIndexSize() const;

    backend::Buffer* vertex_buffer = nullptr;
    backend::Buffer* index_buffer = nullptr;
    
    std::size_t vertex_draw_start = 0;
    std::size_t vertex_draw_count = 0;
    
    std::size_t index_draw_offset = 0;
    std::size_t index_draw_count = 0;
    
    DrawType draw_type = DrawType::ELEMENT;
    PrimitiveType primitive_type = PrimitiveType::TRIANGLE;
    IndexFormat index_format = IndexFormat::U_SHORT;
    
    float line_width = 0.0;
    
    std::size_t index_size = 0;

    std::size_t vertex_capacity = 0;
    std::size_t index_capacity = 0;

    CallBackFunc before_callback = nullptr;
    CallBackFunc after_callback = nullptr;
};

APRIL_END

#endif /* CustomCommand_hpp */
