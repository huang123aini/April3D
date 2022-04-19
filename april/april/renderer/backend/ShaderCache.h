//
//  ShaderCache.hpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/2.
//

#ifndef ShaderCache_hpp
#define ShaderCache_hpp

#include <stdio.h>
#include <string>
#include <unordered_map>

#include "renderer/backend/Macros.h"
#include "base/Ref.h"
#include "platform/AprilPlatformMacros.h"
#include "renderer/backend/ShaderModule.h"


BACKEND_BEGIN

class ShaderCache : public april3d::Ref {
public:
    static ShaderCache* getInstance();
    
    static void destroyInstance();
    
    static backend::ShaderModule* newVertexShaderModule(const std::string& shaderSource);
    
    static backend::ShaderModule* newFragmentShaderModule(const std::string& shaderSource);
    
    void removeUnusedShader();
    
protected:
    virtual ~ShaderCache();
    
    bool init();

    static backend::ShaderModule* newShaderModule(backend::ShaderStage stage, const std::string& shaderSource);
    static std::unordered_map<std::size_t, backend::ShaderModule*> cached_shaders;
    static ShaderCache* shared_shader_cache;
};

BACKEND_END


#endif /* ShaderCache_hpp */
