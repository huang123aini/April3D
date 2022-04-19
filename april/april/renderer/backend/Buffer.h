//
//  Buffer.h
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/2.
//

#ifndef Buffer_h
#define Buffer_h

#include "Macros.h"
#include "Types.h"
#include "base/Ref.h"

namespace april3d {
    class MeshVertexData;
    class MeshIndexData;
}


BACKEND_BEGIN

/**
 * @brief 存储顶点和索引数据
 */
class Buffer : public april3d::Ref {
public:

    virtual void updateData(void* data, std::size_t size) = 0;
    
    virtual void updateSubData(void* data, std::size_t offset, std::size_t size) = 0;

    virtual void usingDefaultStoredData(bool needDefaultStoredData) = 0;

    std::size_t getSize() const { return size_; }

protected:
    Buffer(std::size_t size, backend::BufferType type, backend::BufferUsage usage)
    :usage_(usage)
    ,type_(type)
    ,size_(size) {}
    
    virtual ~Buffer() = default;

    backend::BufferUsage usage_ = backend::BufferUsage::DYNAMIC;
    backend::BufferType type_ = backend::BufferType::VERTEX;
    std::size_t size_ = 0;
};

BACKEND_END

#endif /* Buffer_h */
