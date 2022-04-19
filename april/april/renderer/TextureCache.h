//
//  TextureCache.hpp
//  april3d
//
//  Created by 黄世平 on 2022/4/4.
//

#ifndef TextureCache_hpp
#define TextureCache_hpp

#include <stdio.h>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <queue>
#include <string>
#include <unordered_map>
#include <functional>

#include "base/Ref.h"
#include "renderer/Texture2D.h"
#include "platform/CCImage.h"

#if ENABLE_CACHE_TEXTURE_DATA
    #include <list>
#endif

APRIL_BEGIN

class TextureCache : public Ref {
public:
    // ETC1 ALPHA supports.
    static void setETC1AlphaFileSuffix(const std::string& suffix);
    static std::string getETC1AlphaFileSuffix();

public:
    TextureCache();
    virtual ~TextureCache();
   
    virtual std::string getDescription() const;

    Texture2D* addImage(const std::string &filepath);

    virtual void addImageAsync(const std::string &filepath, const std::function<void(Texture2D*)>& callback);
    
    void addImageAsync(const std::string &path, const std::function<void(Texture2D*)>& callback, const std::string& callbackKey );

    /** Unbind a specified bound image asynchronous callback.
     * In the case an object who was bound to an image asynchronous callback was destroyed before the callback is invoked,
     * the object always need to unbind this callback manually.
     * @param filename It's the related/absolute path of the file image.
     * @since v3.1
     */
    virtual void unbindImageAsync(const std::string &filename);
    
    virtual void unbindAllImageAsync();

    Texture2D* addImage(Image *image, const std::string &key);

    Texture2D* getTextureForKey(const std::string& key) const;

    bool reloadTexture(const std::string& fileName);

    void removeAllTextures();

    void removeUnusedTextures();

    void removeTexture(Texture2D* texture);
    
    void removeTextureForKey(const std::string &key);

    std::string getCachedTextureInfo() const;

    //Wait for texture cache to quit before destroy instance.
    /**Called by director, please do not called outside.*/
    void waitForQuit();

    /**
     * Get the file path of the texture
     *
     * @param texture A Texture2D object pointer.
     *
     * @return The full path of the file.
     */
    std::string getTextureFilePath(Texture2D* texture) const;

    /** Reload texture from a new file.
    * This function is mainly for editor, won't suggest use it in game for performance reason.
    *
    * @param srcName Original texture file name.
    * @param dstName New texture file name.
    *
    * @since v3.10
    */
    void renameTextureWithKey(const std::string& srcName, const std::string& dstName);


private:
    void addImageAsyncCallBack(float dt);
    void loadImage();
    void parseNinePatchImage(Image* image, Texture2D* texture, const std::string& path);
public:
protected:
    struct AsyncStruct;
    
    std::thread* loading_thread;

    std::deque<AsyncStruct*> async_struct_queue;
    std::deque<AsyncStruct*> request_queue;
    std::deque<AsyncStruct*> response_queue;

    std::mutex request_mutex;
    std::mutex response_mutex;
    
    std::condition_variable sleep_condition;

    bool need_quit;

    int async_ref_count;

    std::unordered_map<std::string, Texture2D*> textures_;

    static std::string s_etc1AlphaFileSuffix;
};

#if ENABLE_CACHE_TEXTURE_DATA

class VolatileTexture {
    typedef enum {
        kInvalid = 0,
        kImageFile,
        kImageData,
        kString,
        kImage,
    }ccCachedImageType;

private:
    VolatileTexture(Texture2D *t);
  
    ~VolatileTexture();

protected:
    friend class  VolatileTextureMgr;
    Texture2D *_texture;
    
    Image *_uiImage;

    ccCachedImageType _cashedImageType;

    void *_textureData;
    int  _dataLen;
    Size _textureSize;
    backend::PixelFormat _pixelFormat;

    std::string _fileName;

    std::string               _text;
    FontDefinition            _fontDefinition;
};

class  VolatileTextureMgr {
public:
    static void addImageTexture(Texture2D *tt, const std::string& imageFileName);
    static void addStringTexture(Texture2D *tt, const char* text, const FontDefinition& fontDefinition);
    static void addDataTexture(Texture2D *tt, void* data, int dataLen, backend::PixelFormat pixelFormat, const Size& contentSize);
    static void addImage(Texture2D *tt, Image *image);
    static void removeTexture(Texture2D *t);
    static void reloadAllTextures();

public:
    static std::list<VolatileTexture*> _textures;
    static bool _isReloading;

    // find VolatileTexture by Texture2D*
    // if not found, create a new one
    static VolatileTexture* findVolotileTexture(Texture2D *tt);

private:
    static void reloadTexture(Texture2D* texture, const std::string& filename, backend::PixelFormat pixelFormat);
};

#endif

APRIL_END

#endif /* TextureCache_hpp */
