//
//  ShaderCache.cpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/2.
//

#include "ShaderCache.h"

#include "ShaderCache.h"
#include "renderer/backend/Device.h"

BACKEND_BEGIN

std::unordered_map<std::size_t, backend::ShaderModule*> ShaderCache::cached_shaders;
ShaderCache* ShaderCache::shared_shader_cache = nullptr;

ShaderCache* ShaderCache::getInstance() {
    if(!shared_shader_cache) {
        shared_shader_cache = new (std::nothrow) ShaderCache();
        if(!shared_shader_cache->init()) {
            SAFE_RELEASE(shared_shader_cache);
        }
    }
    return shared_shader_cache;
}

void ShaderCache::destroyInstance() {
    SAFE_RELEASE_NULL(shared_shader_cache);
}

ShaderCache::~ShaderCache() {
    for(auto& shaderModule : cached_shaders) {
        SAFE_RELEASE(shaderModule.second);
    }
    //CCLOGINFO("deallocing ProgramCache: %p", this);
}

bool ShaderCache::init() {
    return true;
}

backend::ShaderModule* ShaderCache::newVertexShaderModule(const std::string& shaderSource) {
    auto vertexShaderModule = newShaderModule(backend::ShaderStage::VERTEX, shaderSource);
    return vertexShaderModule;
}

backend::ShaderModule* ShaderCache::newFragmentShaderModule(const std::string& shaderSource) {
    auto fragmenShaderModule = newShaderModule(backend::ShaderStage::FRAGMENT, shaderSource);
    return fragmenShaderModule;
}

backend::ShaderModule* ShaderCache::newShaderModule(backend::ShaderStage stage, const std::string& shaderSource) {
    std::size_t key = std::hash<std::string>{}(shaderSource);
    auto iter = cached_shaders.find(key);
    if (cached_shaders.end() != iter) {
        return iter->second;
    }
    /**
     *生成shader,并缓存
     */
    auto shader = backend::Device::getInstance()->newShaderModule(stage, shaderSource);
    shader->setHashValue(key);
    cached_shaders.emplace(key, shader);
    return shader;
}

void ShaderCache::removeUnusedShader() {
    for (auto iter = cached_shaders.cbegin(); iter != cached_shaders.cend();) {
        auto shaderModule = iter->second;
        if (shaderModule->getReferenceCount() == 1) {
            shaderModule->release();
            iter = cached_shaders.erase(iter);
        } else {
            ++iter;
        }
    }
}

BACKEND_END
