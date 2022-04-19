//
//  BufferGL.cpp
//  april3d
//
//  Created by 黄世平 on 2022/4/3.
//

#include <cassert>

#include "BufferGL.h"
#include "base/aprilMacros.h"

BACKEND_BEGIN

GLenum toGLUsage(const BufferUsage& usage) {
    switch (usage) {
        case BufferUsage::STATIC:
            return GL_STATIC_DRAW;
        case BufferUsage::DYNAMIC:
            return GL_DYNAMIC_DRAW;
        default:
            return GL_DYNAMIC_DRAW;
    }
}

BufferGL::BufferGL(std::size_t size, BufferType type, BufferUsage usage)
: Buffer(size, type, usage) {
    glGenBuffers(1, &buffer_);
}

BufferGL::~BufferGL() {
    if (buffer_) {
        glDeleteBuffers(1, &buffer_);
    }
}

void BufferGL::usingDefaultStoredData(bool needDefaultStoredData) {}

void BufferGL::updateData(void* data, std::size_t size) {
    assert(size && size <= size_);
    
    if (buffer_) {
        if (BufferType::VERTEX == type_) {
            glBindBuffer(GL_ARRAY_BUFFER, buffer_);
            glBufferData(GL_ARRAY_BUFFER, size, data, toGLUsage(usage_));
        } else {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, toGLUsage(usage_));
        }
        CHECK_GL_ERROR_DEBUG();
        buffer_allocated = size;
    }
}

void BufferGL::updateSubData(void* data, std::size_t offset, std::size_t size) {
    if (buffer_allocated == 0) {
        printf("updateData should be invoke before updateSubData \n");
        return;
    }
    if (offset + size > buffer_allocated) {
        printf("buffer size overflow \n");
        return;
    }
    if (buffer_) {
        CHECK_GL_ERROR_DEBUG();
        if (BufferType::VERTEX == type_) {
            /**
             *顶点数据
             */
            glBindBuffer(GL_ARRAY_BUFFER, buffer_);
            glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
        } else {
            /**
             *索引数据
             */
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_);
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
        }
        CHECK_GL_ERROR_DEBUG();
    }
}

BACKEND_END
