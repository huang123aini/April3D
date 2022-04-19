//
//  Texture2D.hpp
//  april3d
//
//  Created by 黄世平 on 2022/4/4.
//

#ifndef Texture2D_hpp
#define Texture2D_hpp

#include <stdio.h>
#include <string>
#include <map>
#include <unordered_map>

#include "base/Ref.h"
#include "math/Geometry.h"
#include "base/aprilTypes.h"
#include "renderer/CustomCommand.h"

APRIL_BEGIN

class Image;
class NinePatchInfo;
class SpriteFrame;
typedef struct _MipmapInfo MipmapInfo;

namespace ui {
    class Scale9Sprite;
}

namespace backend {
    class Texture2DBackend;
    class TextureBackend;
    class ProgramState;
}

class Texture2D : public Ref {
public:
    struct PixelFormatInfo {

        PixelFormatInfo(int aBpp, bool aCompressed, bool anAlpha)
            : bpp_(aBpp)
            , compressed_(aCompressed)
            , alpha_(anAlpha) {}
        
        int bpp_;
        bool compressed_;
        bool alpha_;
    };
    
    typedef std::map<backend::PixelFormat, const PixelFormatInfo> PixelFormatInfoMap;
    
    using TexParams = backend::SamplerDescriptor;

public:
  
    static void setDefaultAlphaPixelFormat(backend::PixelFormat format);

    static backend::PixelFormat getDefaultAlphaPixelFormat();
    
public:
    Texture2D();
    virtual ~Texture2D();

    bool initWithData(const void *data, ssize_t dataLen, backend::PixelFormat pixelFormat, int pixelsWide, int pixelsHigh, const Size& contentSize, bool preMultipliedAlpha = false) { return initWithData(data, dataLen, pixelFormat, pixelFormat, pixelsWide, pixelsHigh, contentSize, preMultipliedAlpha);}

    bool initWithData(const void *data, ssize_t dataLen, backend::PixelFormat pixelFormat, backend::PixelFormat renderFormat, int pixelsWide, int pixelsHigh, const Size& contentSize, bool preMultipliedAlpha = false);
    
    bool initWithMipmaps(MipmapInfo* mipmaps, int mipmapsNum, backend::PixelFormat pixelFormat, backend::PixelFormat renderFormat, int pixelsWide, int pixelsHigh, bool preMultipliedAlpha = false);
    
    bool updateWithData(void *data,int offsetX,int offsetY,int width,int height);
   
    void drawAtPoint(const Vec2& point, float globalZOrder);
    
    void drawInRect(const Rect& rect, float globalZOrder);

    bool initWithImage(Image* image);
    
    bool initWithImage(Image* image, backend::PixelFormat format);

    bool initWithString(const char *text,  const std::string &fontName, float fontSize, const Size& dimensions = Size(0, 0), TextHAlignment hAlignment = TextHAlignment::CENTER, TextVAlignment vAlignment = TextVAlignment::TOP, bool enableWrap = true, int overflow = 0);

    bool initWithString(const char *text, const FontDefinition& textDefinition);
    
    void setRenderTarget(bool renderTarget);
    inline bool isRenderTarget() const { return _isRenderTarget; }


    void setTexParameters(const TexParams &params);
    
    void generateMipmap();
    
    void setAntiAliasTexParameters();

    void setAliasTexParameters();

    const char* getStringForFormat() const;

    unsigned int getBitsPerPixelForFormat() const;

    unsigned int getBitsPerPixelForFormat(backend::PixelFormat format) const;

    const Size& getContentSizeInPixels();

    bool hasPremultipliedAlpha() const;
    
    bool hasMipmaps() const;

    backend::PixelFormat getPixelFormat() const;
    
    int getPixelsWidth() const;
    
    int getPixelsHeight() const;
    
    backend::TextureBackend* getBackendTexture() const;
    
    float getMaxS() const;
    
    void setMaxS(float maxS);
  
    float getMaxT() const;

    void setMaxT(float maxT);
    
    Size getContentSize() const;

    std::string getPath()const { return _filePath; }

    void setAlphaTexture(Texture2D* alphaTexture);
    
    Texture2D* getAlphaTexture() const;

    bool getAlphaTextureName() const;
    
public:

    static const PixelFormatInfoMap& getPixelFormatInfoMap();
    
    void initProgram();
   
protected:
    backend::PixelFormat pixel_format;

    int pixels_width;

    int pixels_Hight;

    backend::Texture2DBackend* texture_;
    
    float max_s;
    
    float max_t;

    Size content_size;

    bool has_premultiplied_alpha;
    
    bool has_mip_maps;

    static const PixelFormatInfoMap pixel_format_info_tables;

    bool antialias_enabled;
    
    NinePatchInfo* nine_patch_info;
 
    friend class SpriteFrameCache;
    
    friend class TextureCache;
    
    friend class ui::Scale9Sprite;

    bool valid_;
    std::string file_path;

    Texture2D* alpha_texture;

    backend::ProgramState* program_state = nullptr;

    backend::UniformLocation mvp_matrix_location;

    backend::UniformLocation texture_location;

    CustomCommand custom_command;
    
    bool is_render_target = false;
};


APRIL_END


#endif /* Texture2D_hpp */
