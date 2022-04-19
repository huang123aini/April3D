//
//  TextureAtlas.cpp
//  april3d
//
//  Created by 黄世平 on 2022/4/4.

#include "renderer/TextureAtlas.h"

#include <stdlib.h>

#include "base/aprilMacros.h"
#include "base/ccUTF8.h"
#include "base/EventType.h"
#include "base/CCDirector.h"
#include "base/Configuration.h"
#include "base/EventDispatcher.h"
#include "base/EventListenerCustom.h"
#include "renderer/TextureCache.h"
#include "renderer/CCRenderer.h"
#include "renderer/Texture2D.h"

// implementation TextureAtlas

APRIL_BEGIN

TextureAtlas::TextureAtlas() {}

TextureAtlas::~TextureAtlas() {
    printf("deallocing TextureAtlas: %p \n", this);

    SAFE_FREE(quads_);
    SAFE_FREE(indices_);
    SAFE_RELEASE(texture_);
    
#if ENABLE_CACHEtexture_DATA
    Director::getInstance()->getEventDispatcher()->removeEventListener(_rendererRecreatedListener);
#endif
}

size_t TextureAtlas::getTotalQuads() const {
    return total_quads;
}

size_t TextureAtlas::getCapacity() const {
    return capacity_;
}

Texture2D* TextureAtlas::getTexture() const {
    return texture_;
}

void TextureAtlas::setTexture(Texture2D * var) {
    SAFE_RETAIN(var);
    SAFE_RELEASE(texture_);
    texture = var;
}

V3F_C4B_T2F_Quad* TextureAtlas::getQuads() {
    //if someone accesses the quads directly, presume that changes will be made
    dirty_ = true;
    return quads_;
}

void TextureAtlas::setQuads(V3F_C4B_T2F_Quad* quads) {
    quads_ = quads;
}

// TextureAtlas - alloc & init

TextureAtlas * TextureAtlas::create(const std::string& file, ssize_t capacity) {
    TextureAtlas * textureAtlas = new (std::nothrow) TextureAtlas();
    if(textureAtlas && textureAtlas->initWithFile(file, capacity)) {
        textureAtlas->autorelease();
        return textureAtlas;
    }
    SAFE_DELETE(textureAtlas);
    return nullptr;
}

TextureAtlas * TextureAtlas::createWithTexture(Texture2D *texture, ssize_t capacity) {
    TextureAtlas * textureAtlas = new (std::nothrow) TextureAtlas();
    if (textureAtlas && textureAtlas->initWithTexture(texture, capacity)) {
        textureAtlas->autorelease();
        return textureAtlas;
    }
    SAFE_DELETE(textureAtlas);
    return nullptr;
}

bool TextureAtlas::initWithFile(const std::string& file, ssize_t capacity) {
    // retained in property
    Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(file);

    if (texture) {
        return initWithTexture(texture, capacity);
    } else {
        printf("cocos2d: Could not open file: %s \n", file.c_str());
        return false;
    }
}

bool TextureAtlas::initWithTexture(Texture2D *texture, ssize_t capacity) {
    CCASSERT(capacity>=0, "Capacity must be >= 0");
    
    capacity_ = capacity;
    total_quads = 0;

    // retained in property
    this->texture_ = texture;
    SAFE_RETAIN(texture);

    // Re-initialization is not allowed
    /**CCASSERT(quads_ == nullptr && indices_ == nullptr, "quads_ and indices_ should be nullptr.");*/

    quads_ = (V3F_C4B_T2F_Quad*)malloc( capacity_ * sizeof(V3F_C4B_T2F_Quad) );
    indices_ = (uint16_t *)malloc( capacity_ * 6 * sizeof(uint16_t) );
    
    if( ! ( quads_ && indices_) && capacity_ > 0) {
        SAFE_FREE(quads_);
        SAFE_FREE(indices_);
        SAFE_RELEASE_NULL(texture_);
        return false;
    }

    memset( quads_, 0, capacity_ * sizeof(V3F_C4B_T2F_Quad) );
    memset( indices_, 0, capacity_ * 6 * sizeof(uint16_t) );
    this->setupIndices();

    dirty_ = true;

    return true;
}

std::string TextureAtlas::getDescription() const {
    return StringUtils::format("<TextureAtlas | totalQuads = %d>", static_cast<int>(total_quads));
}

void TextureAtlas::setupIndices() {
    if (capacity_ == 0) {
        return;
    }

    for( int i=0; i < capacity_; i++) {
        indices_[i*6+0] = i*4+0;
        indices_[i*6+1] = i*4+1;
        indices_[i*6+2] = i*4+2;

        // inverted index. issue #179
        indices_[i*6+3] = i*4+3;
        indices_[i*6+4] = i*4+2;
        indices_[i*6+5] = i*4+1;
    }
}


// TextureAtlas - Update, Insert, Move & Remove

void TextureAtlas::updateQuad(V3F_C4B_T2F_Quad *quad, ssize_t index) {
    if( index < 0 || index >= capacity_) {
        printf("updateQuadWithTexture: Invalid index \n");
        return;
    }
    total_quads = MAX( index+1, total_quads);
    quads_[index] = *quad;
    dirty_ = true;

}

void TextureAtlas::insertQuad(V3F_C4B_T2F_Quad *quad, ssize_t index) {
   /** CCASSERT( index>=0 && index<capacity_, "insertQuadWithTexture: Invalid index");*/

    total_quads++;
   /** CCASSERT( total_quads <= capacity_, "invalid totalQuads");*/

    // issue #575. index can be > totalQuads
    auto remaining = (total_quads-1) - index;

    // last object doesn't need to be moved
    if( remaining > 0) {
        // texture coordinates
        memmove( &quads_[index+1],&quads_[index], sizeof(quads_[0]) * remaining );
    }

    quads_[index] = *quad;


    dirty_ = true;

}

void TextureAtlas::insertQuads(V3F_C4B_T2F_Quad* quads, ssize_t index, ssize_t amount)
{
    CCASSERT(index>=0 && amount>=0 && index+amount<=capacity_, "insertQuadWithTexture: Invalid index + amount");

    total_quads += amount;

    CCASSERT( total_quads <= capacity_, "invalid totalQuads");

    // issue #575. index can be > totalQuads
    auto remaining = (total_quads-1) - index - amount;

    // last object doesn't need to be moved
    if( remaining > 0)
    {
        // tex coordinates
        memmove( &quads_[index+amount],&quads_[index], sizeof(quads_[0]) * remaining );
    }


    auto max = index + amount;
    int j = 0;
    for (ssize_t i = index; i < max ; i++)
    {
        quads_[index] = quads[j];
        index++;
        j++;
    }

    dirty_ = true;
}

void TextureAtlas::insertQuadFromIndex(ssize_t oldIndex, ssize_t newIndex)
{
    CCASSERT( newIndex >= 0 && newIndex < total_quads, "insertQuadFromIndex:atIndex: Invalid index");
    CCASSERT( oldIndex >= 0 && oldIndex < total_quads, "insertQuadFromIndex:atIndex: Invalid index");

    if( oldIndex == newIndex )
    {
        return;
    }
    // because it is ambiguous in iphone, so we implement abs ourselves
    // unsigned int howMany = std::abs( oldIndex - newIndex);
    auto howMany = (oldIndex - newIndex) > 0 ? (oldIndex - newIndex) :  (newIndex - oldIndex);
    auto dst = oldIndex;
    auto src = oldIndex + 1;
    if( oldIndex > newIndex)
    {
        dst = newIndex+1;
        src = newIndex;
    }

    // texture coordinates
    V3F_C4B_T2F_Quad quadsBackup = quads_[oldIndex];
    memmove( &quads_[dst],&quads_[src], sizeof(quads_[0]) * howMany );
    quads_[newIndex] = quadsBackup;


    dirty_ = true;
}

void TextureAtlas::removeQuadAtIndex(ssize_t index)
{
    CCASSERT( index>=0 && index<total_quads, "removeQuadAtIndex: Invalid index");

    auto remaining = (total_quads-1) - index;

    // last object doesn't need to be moved
    if( remaining )
    {
        // texture coordinates
        memmove( &quads_[index],&quads_[index+1], sizeof(quads_[0]) * remaining );
    }

    total_quads--;


    dirty_ = true;
}

void TextureAtlas::removeQuadsAtIndex(ssize_t index, ssize_t amount)
{
    CCASSERT(index>=0 && amount>=0 && index+amount<=total_quads, "removeQuadAtIndex: index + amount out of bounds");

    auto remaining = (total_quads) - (index + amount);

    total_quads -= amount;

    if ( remaining )
    {
        memmove( &quads_[index], &quads_[index+amount], sizeof(quads_[0]) * remaining );
    }

    dirty_ = true;
}

void TextureAtlas::removeAllQuads()
{
    total_quads = 0;
}

// TextureAtlas - Resize
bool TextureAtlas::resizeCapacity(ssize_t newCapacity)
{
    CCASSERT(newCapacity >= 0, "capacity >= 0");
    if (newCapacity == capacity_)
    {
        return true;
    }
    auto oldCapacity = capacity_;

    // update capacity and totalQuads
    total_quads = MIN(total_quads, newCapacity);
    capacity_ = newCapacity;

    V3F_C4B_T2F_Quad* tmpQuads = nullptr;
    uint16_t* tmpIndices = nullptr;

    // when calling initWithTexture(fileName, 0) on bada device, calloc(0, 1) will fail and return nullptr,
    // so here must judge whether quads_ and indices_ is nullptr.

    ssize_t quads__size = sizeof(quads_[0]);
    ssize_t newquads__size = capacity_ * quads__size;
    if (quads_ == nullptr)
    {
        tmpQuads = (V3F_C4B_T2F_Quad*)malloc(newquads__size);
        if (tmpQuads != nullptr)
        {
            memset(tmpQuads, 0, newquads__size);
        }
    }
    else
    {
        tmpQuads = (V3F_C4B_T2F_Quad*)realloc(quads_, newquads__size);
        if (tmpQuads != nullptr && capacity_ > oldCapacity)
        {
            memset(tmpQuads + oldCapacity, 0, (capacity_ - oldCapacity)*quads__size);
        }
        quads_ = nullptr;
    }

    ssize_t indices__size = sizeof(indices_[0]);
    ssize_t new_size = capacity_ * 6 * indices__size;

    if (indices_ == nullptr)
    {
        tmpIndices = (uint16_t*)malloc(new_size);
        if (tmpIndices != nullptr)
        {
            memset(tmpIndices, 0, new_size);
        }
    }
    else
    {
        tmpIndices = (uint16_t*)realloc(indices_, new_size);
        if (tmpIndices != nullptr && capacity_ > oldCapacity)
        {
            memset(tmpIndices + oldCapacity, 0, (capacity_ - oldCapacity) * 6 * indices__size);
        }
        indices_ = nullptr;
    }

    if (!(tmpQuads && tmpIndices)) {
        CCLOG("cocos2d: TextureAtlas: not enough memory");
        CC_SAFE_FREE(tmpQuads);
        CC_SAFE_FREE(tmpIndices);
        CC_SAFE_FREE(quads_);
        CC_SAFE_FREE(indices_);
        capacity_ = total_quads = 0;
        return false;
    }

    quads_ = tmpQuads;
    indices_ = tmpIndices;

    setupIndices();

    dirty_ = true;

    return true;
}


void TextureAtlas::increaseTotalQuadsWith(ssize_t amount)
{
    CCASSERT(amount>=0, "amount >= 0");
    total_quads += amount;
}

void TextureAtlas::moveQuadsFromIndex(ssize_t oldIndex, ssize_t amount, ssize_t newIndex)
{
    CCASSERT(oldIndex>=0 && amount>=0 && newIndex>=0, "values must be >= 0");
    CCASSERT(newIndex + amount <= total_quads, "insertQuadFromIndex:atIndex: Invalid index");
    CCASSERT(oldIndex < total_quads, "insertQuadFromIndex:atIndex: Invalid index");

    if( oldIndex == newIndex )
    {
        return;
    }
    //create buffer
    size_t quadSize = sizeof(V3F_C4B_T2F_Quad);
    V3F_C4B_T2F_Quad* tempQuads = (V3F_C4B_T2F_Quad*)malloc( quadSize * amount);
    memcpy( tempQuads, &quads_[oldIndex], quadSize * amount );

    if (newIndex < oldIndex)
    {
        // move quads from newIndex to newIndex + amount to make room for buffer
        memmove( &quads_[newIndex], &quads_[newIndex+amount], (oldIndex-newIndex)*quadSize);
    }
    else
    {
        // move quads above back
        memmove( &quads_[oldIndex], &quads_[oldIndex+amount], (newIndex-oldIndex)*quadSize);
    }
    memcpy( &quads_[newIndex], tempQuads, amount*quadSize);

    free(tempQuads);

    dirty_ = true;
}

void TextureAtlas::moveQuadsFromIndex(ssize_t index, ssize_t newIndex)
{
    CCASSERT(index>=0 && newIndex>=0, "values must be >= 0");
    CCASSERT(newIndex + (total_quads - index) <= capacity_, "moveQuadsFromIndex move is out of bounds");

    memmove(quads_ + newIndex,quads_ + index, (total_quads - index) * sizeof(quads_[0]));
}

void TextureAtlas::fillWithEmptyQuadsFromIndex(ssize_t index, ssize_t amount) {
    CCASSERT(index>=0 && amount>=0, "values must be >= 0");
    V3F_C4B_T2F_Quad quad;
    memset(&quad, 0, sizeof(quad));

    auto to = index + amount;
    for (ssize_t i = index ; i < to ; i++) {
        quads_[i] = quad;
    }
}

APRIL_END

