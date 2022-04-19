//
//  TextureGL.cpp
//  april3d
//
//  Created by 黄世平 on 2022/4/3.
//

#include "TextureGL.h"
#include "base/aprilMacros.h"

#include "platform/AprilPlatformConfig.h"
#include "UtilsGL.h"

#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>

BACKEND_BEGIN

#define ISPOW2(n) (((n) & (n-1)) == 0)

namespace {
    bool isMipmapEnabled(GLint filter) {
        switch(filter) {
            case GL_LINEAR_MIPMAP_LINEAR:
            case GL_LINEAR_MIPMAP_NEAREST:
            case GL_NEAREST_MIPMAP_NEAREST:
            case GL_NEAREST_MIPMAP_LINEAR:
                return true;
            default:
                break;
        }
        return false;
    }
}

void TextureInfoGL::applySamplerDescriptor(const SamplerDescriptor& descriptor, bool isPow2, bool hasMipmaps) {
    if (descriptor.mag_filter != SamplerFilter::DONT_CARE) {
        mag_filter_gl = UtilsGL::toGLMagFilter(descriptor.mag_filter);
    }

    if (descriptor.min_filter != SamplerFilter::DONT_CARE) {
        min_filter_gl = UtilsGL::toGLMinFilter(descriptor.min_filter, hasMipmaps, isPow2);
    }

    if (descriptor.s_address_mode != SamplerAddressMode::DONT_CARE) {
        s_address_mode_gl = UtilsGL::toGLAddressMode(descriptor.s_address_mode, isPow2);
    }

    if (descriptor.t_address_mode != SamplerAddressMode::DONT_CARE) {
        t_address_mode_gl = UtilsGL::toGLAddressMode(descriptor.t_address_mode, isPow2);
    }
}

Texture2DGL::Texture2DGL(const TextureDescriptor& descriptor) : Texture2DBackend(descriptor) {
    /**
     *生成纹理
     */
    glGenTextures(1, &texture_info.texture_);
    updateTextureDescriptor(descriptor);

#if ENABLE_CACHE_TEXTURE_DATA
    // Listen this event to restored texture id after coming to foreground on Android.
    back_to_foreground_listener = EventListenerCustom::create(EVENT_RENDERER_RECREATED, [this](EventCustom*){
        glGenTextures(1, &(this->texture_info.texture_));
        this->initWithZeros();
    });
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_backToForegroundListener, -1);
#endif
}

void Texture2DGL::initWithZeros() {
    auto size = width_ * height_ * bits_per_element / 8;
    uint8_t* data = (uint8_t*)malloc(size);
    memset(data, 0, size);
    updateData(data, width_, height_, 0);
    free(data);
}

void Texture2DGL::updateTextureDescriptor(const backend::TextureDescriptor &descriptor) {
    TextureBackend::updateTextureDescriptor(descriptor);
    UtilsGL::toGLTypes(descriptor.texture_format, texture_info.internal_format, texture_info.format_, texture_info.type_, is_compressed);

    bool isPow2 = ISPOW2(width_) && ISPOW2(height_);
    texture_info.mag_filter_gl = UtilsGL::toGLMagFilter(descriptor.sampler_descriptor.mag_filter);
    texture_info.min_filter_gl = UtilsGL::toGLMinFilter(descriptor.sampler_descriptor.min_filter, has_mip_maps, isPow2);

    texture_info.s_address_mode_gl = UtilsGL::toGLAddressMode(descriptor.sampler_descriptor.s_address_mode, isPow2);
    texture_info.t_address_mode_gl = UtilsGL::toGLAddressMode(descriptor.sampler_descriptor.t_address_mode, isPow2);

    updateSamplerDescriptor(descriptor.sampler_descriptor);

    // Update data here because `updateData()` may not be invoked later.
    // For example, a texture used as depth buffer will not invoke updateData().
    initWithZeros();
}

Texture2DGL::~Texture2DGL() {
    if (texture_info.texture_)
        glDeleteTextures(1, &texture_info.texture_);
    texture_info.texture_ = 0;
#if ENABLE_CACHE_TEXTURE_DATA
    Director::getInstance()->getEventDispatcher()->removeEventListener(_backToForegroundListener);
#endif
}

void Texture2DGL::updateSamplerDescriptor(const SamplerDescriptor &sampler) {
    bool isPow2 = ISPOW2(width_) && ISPOW2(height_);
    texture_info.applySamplerDescriptor(sampler, isPow2, has_mip_maps);

    /**
     *激活纹理
     */
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_info.texture_);

    if (sampler.mag_filter != SamplerFilter::DONT_CARE) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture_info.mag_filter_gl);
    }

    if (sampler.min_filter != SamplerFilter::DONT_CARE) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture_info.min_filter_gl);
    }

    if (sampler.s_address_mode != SamplerAddressMode::DONT_CARE) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture_info.s_address_mode_gl);
    }

    if (sampler.t_address_mode != SamplerAddressMode::DONT_CARE) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture_info.t_address_mode_gl);
    }
}

void Texture2DGL::updateData(uint8_t* data, std::size_t width , std::size_t height, std::size_t level) {
    /**
     *Set the row align only when mipmapsNum == 1 and the data is uncompressed
     */
    auto mipmapEnalbed = isMipmapEnabled(texture_info.min_filter_gl) || isMipmapEnabled(texture_info.mag_filter_gl);
    if(!mipmapEnalbed) {
        unsigned int bytesPerRow = static_cast<unsigned int>(width * bits_per_element / 8);

        if(bytesPerRow % 8 == 0) {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 8);
        } else if(bytesPerRow % 4 == 0) {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        } else if(bytesPerRow % 2 == 0) {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
        } else {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        }
    } else {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    }
    glActiveTexture(GL_TEXTURE0);
    /**
     *设置纹理采样参数
     */
    glBindTexture(GL_TEXTURE_2D, texture_info.texture_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture_info.mag_filter_gl);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture_info.min_filter_gl);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture_info.s_address_mode_gl);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture_info.t_address_mode_gl);

    /**
     *加载裸数据data
     */
    glTexImage2D(GL_TEXTURE_2D,
                level,
                texture_info.internal_format,
                width_,
                height_,
                0,
                 texture_info.format_,
                 texture_info.type_,
                data);
    CHECK_GL_ERROR_DEBUG();

    if(!has_mip_maps && level > 0) {
        has_mip_maps = true;
    }
}

void Texture2DGL::updateCompressedData(uint8_t *data, std::size_t width, std::size_t height,
                                       std::size_t dataLen, std::size_t level) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_info.texture_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture_info.mag_filter_gl);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture_info.min_filter_gl);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture_info.s_address_mode_gl);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture_info.t_address_mode_gl);


    glCompressedTexImage2D(GL_TEXTURE_2D,
                           level,
                           texture_info.internal_format,
                           (GLsizei)width,
                           (GLsizei)height,
                           0,
                           dataLen,
                           data);
    CHECK_GL_ERROR_DEBUG();

    if(!has_mip_maps && level > 0)
        has_mip_maps = true;
}

void Texture2DGL::updateSubData(std::size_t xoffset, std::size_t yoffset, std::size_t width, std::size_t height, std::size_t level, uint8_t* data)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_info.texture_);

    glTexSubImage2D(GL_TEXTURE_2D,
                    level,
                    xoffset,
                    yoffset,
                    width,
                    height,
                    texture_info.format_,
                    texture_info.type_,
                    data);
    CHECK_GL_ERROR_DEBUG();

    if(!has_mip_maps && level > 0) {
        has_mip_maps = true;
    }
}

void Texture2DGL::updateCompressedSubData(std::size_t xoffset, std::size_t yoffset, std::size_t width,
                                          std::size_t height, std::size_t dataLen, std::size_t level,
                                          uint8_t *data) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_info.texture_);

    glCompressedTexSubImage2D(GL_TEXTURE_2D,
                              level,
                              xoffset,
                              yoffset,
                              width,
                              height,
                              texture_info.format_,
                              dataLen,
                              data);
    CHECK_GL_ERROR_DEBUG();

    if(!has_mip_maps && level > 0)
        has_mip_maps = true;
}

void Texture2DGL::apply(int index) const {
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, texture_info.texture_);
}

void Texture2DGL::generateMipmaps() {
    if (TextureUsage::RENDER_TARGET == texture_usage) {
        return;
    }
    if(!has_mip_maps) {
        has_mip_maps = true;
        glBindTexture(GL_TEXTURE_2D, texture_info.texture_);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
}

void Texture2DGL::getBytes(std::size_t x, std::size_t y, std::size_t width, std::size_t height, bool flipImage, std::function<void(const unsigned char*, std::size_t, std::size_t)> callback) {
    GLint defaultFBO = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &defaultFBO);

    GLuint frameBuffer = 0;
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_info.texture_, 0);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);

    auto bytePerRow = width * bits_per_element / 8;
    unsigned char* image = new unsigned char[bytePerRow * height];
    glReadPixels(x,y,width, height,GL_RGBA,GL_UNSIGNED_BYTE, image);

    if(flipImage) {
        unsigned char* flippedImage = new unsigned char[bytePerRow * height];
        for (int i = 0; i < height; ++i)
        {
            memcpy(&flippedImage[i * bytePerRow],
                   &image[(height - i - 1) * bytePerRow],
                   bytePerRow);
        }
        callback(flippedImage, width, height);
        SAFE_DELETE_ARRAY(flippedImage);
    } else {
        callback(image, width, height);
        SAFE_DELETE_ARRAY(image);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFBO);
    glDeleteFramebuffers(1, &frameBuffer);
}

TextureCubeGL::TextureCubeGL(const TextureDescriptor& descriptor)
    :TextureCubemapBackend(descriptor) {
    assert(width_ == height_);
    texture_type = TextureType::TEXTURE_CUBE;
    UtilsGL::toGLTypes(texture_format, texture_info.internal_format, texture_info.format_, texture_info.type_, is_compressed);
    glGenTextures(1, &texture_info.texture_);
    updateSamplerDescriptor(descriptor.sampler_descriptor);
        
    CHECK_GL_ERROR_DEBUG();
}

void TextureCubeGL::setTexParameters() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_info.texture_);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, texture_info.min_filter_gl);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, texture_info.mag_filter_gl);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, texture_info.s_address_mode_gl);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, texture_info.t_address_mode_gl);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void TextureCubeGL::updateTextureDescriptor(const backend::TextureDescriptor &descriptor) {
    UtilsGL::toGLTypes(descriptor.texture_format, texture_info.internal_format, texture_info.format_, texture_info.type_, is_compressed);
    texture_format = descriptor.texture_format;
    updateSamplerDescriptor(descriptor.sampler_descriptor);
}

TextureCubeGL::~TextureCubeGL() {
    if(texture_info.texture_) {
        glDeleteTextures(1, &texture_info.texture_);
        texture_info.texture_ = 0;
    }
}

void TextureCubeGL::updateSamplerDescriptor(const SamplerDescriptor &sampler) {
    texture_info.applySamplerDescriptor(sampler, true, has_mip_maps);
    setTexParameters();
}

void TextureCubeGL::apply(int index) const {
    glActiveTexture(GL_TEXTURE0+ index);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_info.texture_);
    CHECK_GL_ERROR_DEBUG();
}

void TextureCubeGL::updateFaceData(TextureCubeFace side, void *data) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_info.texture_);
    CHECK_GL_ERROR_DEBUG();
    int i = static_cast<int>(side);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
        0,                  // level
        GL_RGBA,            // internal format
        width_,              // width
        height_,              // height
        0,                  // border
        texture_info.internal_format,  // format
        texture_info.type_,  // type
        data);              // pixel data

    CHECK_GL_ERROR_DEBUG();
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void TextureCubeGL::getBytes(std::size_t x, std::size_t y, std::size_t width, std::size_t height, bool flipImage, std::function<void(const unsigned char*, std::size_t, std::size_t)> callback) {
    GLint defaultFBO = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &defaultFBO);
    GLuint frameBuffer = 0;
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP, texture_info.texture_, 0);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);

    auto bytePerRow = width * bits_per_element / 8;
    unsigned char* image = new unsigned char[bytePerRow * height];
    glReadPixels(x,y,width, height,GL_RGBA,GL_UNSIGNED_BYTE, image);

    if(flipImage) {
        unsigned char* flippedImage = new unsigned char[bytePerRow * height];
        for (int i = 0; i < height; ++i) {
            memcpy(&flippedImage[i * bytePerRow],
                   &image[(height - i - 1) * bytePerRow],
                   bytePerRow);
        }
        callback(flippedImage, width, height);
        SAFE_DELETE_ARRAY(flippedImage);
    } else {
        callback(image, width, height);
        SAFE_DELETE_ARRAY(image);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, defaultFBO);
    glDeleteFramebuffers(1, &frameBuffer);
}

void TextureCubeGL::generateMipmaps() {
    if (TextureUsage::RENDER_TARGET == texture_usage) {
        return;
    }
    if(!has_mip_maps) {
        has_mip_maps = true;
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture_info.texture_);
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    }
}

BACKEND_END
