//
//  TextureAtlas.hpp
//  april3d
//
//  Created by 黄世平 on 2022/4/4.
//

#ifndef TextureAtlas_hpp
#define TextureAtlas_hpp

#include <stdio.h>
#include <string>

#include "base/aprilTypes.h"
#include "base/Ref.h"
#include "base/aprilConfig.h"

APRIL_BEGIN

class Texture2D;
class EventCustom;
class EventListenerCustom;


/**
 *Texture Atlas.
*/
class  TextureAtlas : public Ref {
public:
  
    static TextureAtlas* create(const std::string& file , ssize_t capacity);

    static TextureAtlas* createWithTexture(Texture2D *texture, ssize_t capacity);
    
    TextureAtlas();
   
    virtual ~TextureAtlas();

    bool initWithFile(const std::string& file, ssize_t capacity);


    bool initWithTexture(Texture2D *texture, ssize_t capacity);

    void updateQuad(V3F_C4B_T2F_Quad* quad, ssize_t index);

    void insertQuad(V3F_C4B_T2F_Quad* quad, ssize_t index);

    void insertQuads(V3F_C4B_T2F_Quad* quads, ssize_t index, ssize_t amount);

    void insertQuadFromIndex(ssize_t fromIndex, ssize_t newIndex);

    void removeQuadAtIndex(ssize_t index);

    void removeQuadsAtIndex(ssize_t index, ssize_t amount);
  
    void removeAllQuads();

    bool resizeCapacity(ssize_t capacity);

    void increaseTotalQuadsWith(ssize_t amount);
    
    void moveQuadsFromIndex(ssize_t oldIndex, ssize_t amount, ssize_t newIndex);

    void moveQuadsFromIndex(ssize_t index, ssize_t newIndex);

    void fillWithEmptyQuadsFromIndex(ssize_t index, ssize_t amount);

    /**
     *是否需要更新VBO的数组缓冲区
     */
    bool isDirty() { return dirty_; }
    /**
     *指定是否需要更新VBO的数组缓冲区
     */
    void setDirty(bool bDirty) { dirty_ = bDirty; }

    virtual std::string getDescription() const;

    size_t getTotalQuads() const;
    
    size_t getCapacity() const;
   
    Texture2D* getTexture() const;
    
    void setTexture(Texture2D* texture);
    
    V3F_C4B_T2F_Quad* getQuads();
    
    void setQuads(V3F_C4B_T2F_Quad* quads);
    
    inline unsigned short* getIndices() { return indices_; }
    
private:
    friend class ParticleBatchNode;
    friend class AtlasNode;

    void renderCommand();

    void setupIndices();

protected:
    unsigned short* indices_ = nullptr;
    bool dirty_ = false;
    size_t total_quads = 0;
    size_t capacity_ = 0;
    Texture2D* texture_ = nullptr;
    V3F_C4B_T2F_Quad* quads_ = nullptr;
    
#if ENABLE_CACHE_TEXTURE_DATA
    EventListenerCustom* _rendererRecreatedListener = nullptr;
#endif
};

APRIL_END

#endif /* TextureAtlas_hpp */
