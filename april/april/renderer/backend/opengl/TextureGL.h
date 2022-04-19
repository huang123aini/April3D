//
//  TextureGL.hpp
//  april3d
//
//  Created by 黄世平 on 2022/4/3.
//

#ifndef TextureGL_hpp
#define TextureGL_hpp

#include <stdio.h>
#include "../Texture.h"
#include "platform/AprilGL.h"

BACKEND_BEGIN

struct TextureInfoGL {
    void applySamplerDescriptor(const SamplerDescriptor &desc, bool isPow2, bool hasMipmaps);
    GLint mag_filter_gl = GL_LINEAR;
    GLint min_filter_gl = GL_LINEAR;
    GLint s_address_mode_gl = GL_REPEAT;
    GLint t_address_mode_gl = GL_REPEAT;
    /**
     *default for glTexImage2D().
     */
    GLint internal_format = GL_RGBA;
    GLenum format_ = GL_RGBA;
    GLenum type_ = GL_UNSIGNED_BYTE;
    GLuint texture_ = 0;
};

/**
 *2D纹理
 */
class Texture2DGL : public backend::Texture2DBackend {
public:
  
    Texture2DGL(const TextureDescriptor& descriptor);
    ~Texture2DGL();
    
    virtual void updateData(uint8_t* data, std::size_t width , std::size_t height, std::size_t level) override;
    
    virtual void updateCompressedData(uint8_t* data, std::size_t width , std::size_t height, std::size_t dataLen, std::size_t level) override;
    
    virtual void updateSubData(std::size_t xoffset, std::size_t yoffset, std::size_t width, std::size_t height, std::size_t level, uint8_t* data) override;
    
    virtual void updateCompressedSubData(std::size_t xoffset, std::size_t yoffset, std::size_t width, std::size_t height, std::size_t dataLen, std::size_t level, uint8_t* data) override;
    
    virtual void updateSamplerDescriptor(const SamplerDescriptor &sampler)  override;
    
    virtual void getBytes(std::size_t x, std::size_t y, std::size_t width, std::size_t height, bool flipImage, std::function<void(const unsigned char*, std::size_t, std::size_t)> callback) override;
    
    virtual void generateMipmaps() override;

    virtual void updateTextureDescriptor(const TextureDescriptor& descriptor) override;

    inline GLuint getHandler() const { return texture_info.texture_; }

    void apply(int index) const;

private:
    void initWithZeros();

    TextureInfoGL texture_info;
};

/**
 *立方体纹理
 */
class TextureCubeGL: public backend::TextureCubemapBackend {
public:
   
    TextureCubeGL(const TextureDescriptor& descriptor);
    ~TextureCubeGL();
    
    virtual void updateSamplerDescriptor(const SamplerDescriptor &sampler) override;
    
    virtual void updateFaceData(TextureCubeFace side, void *data) override;
    
    virtual void getBytes(std::size_t x, std::size_t y, std::size_t width, std::size_t height, bool flipImage, std::function<void(const unsigned char*, std::size_t, std::size_t)> callback) override;
    
    virtual void generateMipmaps() override;

    virtual void updateTextureDescriptor(const TextureDescriptor& descriptor) override ;

    inline GLuint getHandler() const { return texture_info.texture_; }

    void apply(int index) const;

private:
    void setTexParameters();

    TextureInfoGL texture_info;
};

BACKEND_END

#endif /* TextureGL_hpp */
