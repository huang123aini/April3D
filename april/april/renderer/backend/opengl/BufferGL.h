//
//  BufferGL.hpp
//  april3d
//
//  Created by 黄世平 on 2022/4/3.
//

#ifndef BufferGL_hpp
#define BufferGL_hpp

#include <stdio.h>
#include <vector>
#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>

#include "renderer/backend/Buffer.h"
#include "platform/AprilGL.h"


BACKEND_BEGIN

class BufferGL : public Buffer {
public:

    BufferGL(std::size_t size, BufferType type, BufferUsage usage);
    ~BufferGL();
    
    virtual void updateData(void* data, std::size_t size) override;

    virtual void updateSubData(void* data, std::size_t offset, std::size_t size) override;

    virtual void usingDefaultStoredData(bool needDefaultStoredData) override ;

    inline GLuint getHandler() const { return buffer_; }

private:
    GLuint buffer_ = 0;
    std::size_t buffer_allocated = 0;
    char* data_ = nullptr;
};

BACKEND_END


#endif /* BufferGL_hpp */
