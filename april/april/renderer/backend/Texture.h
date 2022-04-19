//
//  Texture.hpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/2.
//

#ifndef Texture_hpp
#define Texture_hpp

#include <stdio.h>
#include <cassert>
#include <functional>

#include "Types.h"
#include "base/Ref.h"

BACKEND_BEGIN
/**
 * Store texture description.
 */
struct TextureDescriptor
{
    TextureType texture_type = TextureType::TEXTURE_2D;
    PixelFormat texture_format = PixelFormat::RGBA8888;
    TextureUsage texture_usage = TextureUsage::READ;
    uint32_t width_ = 0;
    uint32_t height_ = 0;
    uint32_t depth_ = 0;
    SamplerDescriptor sampler_descriptor;
};

class TextureBackend : public april3d::Ref {
public:
    virtual void updateSamplerDescriptor(const SamplerDescriptor &sampler) = 0;

    virtual void getBytes(std::size_t x, std::size_t y, std::size_t width, std::size_t height, bool flipImage, std::function<void(const unsigned char*, std::size_t, std::size_t)> callback) = 0;
    
    virtual void generateMipmaps() = 0;

    virtual void updateTextureDescriptor(const TextureDescriptor& descriptor);

    inline PixelFormat getTextureFormat() const { return texture_format; }

    inline TextureUsage getTextureUsage() const { return texture_usage; }

    inline TextureType getTextureType() const { return texture_type; }
    
    inline bool hasMipmaps() const { return has_mip_maps; }

protected:

    TextureBackend(const TextureDescriptor& descriptor);
    virtual ~TextureBackend();
        
    uint8_t bits_per_element = 0;
    bool has_mip_maps = false;
    bool is_compressed = false;
    uint32_t width_ = 0;
    uint32_t height_ = 0;

    TextureType  texture_type      = TextureType::TEXTURE_2D;
    PixelFormat  texture_format    = PixelFormat::RGBA8888;
    TextureUsage  texture_usage    = TextureUsage::READ;
};

class Texture2DBackend : public TextureBackend {
public:

    virtual void updateData(uint8_t* data, std::size_t width , std::size_t height, std::size_t level) = 0;
    
    virtual void updateCompressedData(uint8_t* data, std::size_t width , std::size_t height, std::size_t dataLen, std::size_t level) = 0;
    
    virtual void updateSubData(std::size_t xoffset, std::size_t yoffset, std::size_t width, std::size_t height, std::size_t level, uint8_t* data) = 0;
    
    virtual void updateCompressedSubData(std::size_t xoffset, std::size_t yoffset, std::size_t width, std::size_t height, std::size_t dataLen, std::size_t level, uint8_t* data) = 0;

    inline std::size_t getWidth() const { return width_; }

    inline std::size_t getHeight() const { return height_; }

protected:
    Texture2DBackend(const TextureDescriptor& descriptor);
};

class TextureCubemapBackend : public TextureBackend {
public:
 
    virtual void updateFaceData(TextureCubeFace side, void *data) = 0;
        
protected:
    TextureCubemapBackend(const TextureDescriptor& descriptor);
};

BACKEND_END

#endif /* Texture_hpp */
