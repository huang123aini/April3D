//
//  ShaderModule.hpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/2.
//

#ifndef ShaderModule_hpp
#define ShaderModule_hpp

#include <stdio.h>
#include <string>

#include "Macros.h"
#include "Types.h"
#include "base/Ref.h"

BACKEND_BEGIN

enum Uniform : uint32_t {
    MVP_MATRIX,
    TEXTURE0,
    TEXTURE1,
    TEXTURE2,
    TEXTURE3,
    TEXTURE_COLOR,
    EFFECT_TYPE,
    EFFECT_COLOR,
    UNIFORM_MAX //Maximum uniforms
};

enum Attribute : uint32_t {
    POSITION,
    COLOR,
    TEXCOORD0,
    TEXCOORD1,
    TEXCOORD2,
    TEXCOORD3,
    ATTRIBUTE_MAX //Maximum attributes
};

class ShaderModule : public april3d::Ref {
public:
    
    ShaderStage getShaderStage() const;

    std::size_t getHashValue() const { return hash_; }
    
protected:
    ShaderModule(ShaderStage stage);
    virtual ~ShaderModule();
    inline void setHashValue(std::size_t hash) { hash_ = hash; }
    
    friend class ShaderCache;
    ShaderStage stage_ = ShaderStage::VERTEX;
    std::size_t hash_ = 0;
};

BACKEND_END


#endif /* ShaderModule_hpp */
