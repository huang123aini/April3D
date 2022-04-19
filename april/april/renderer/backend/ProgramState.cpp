#include "renderer/backend/ProgramState.h"
#include "renderer/backend/ProgramCache.h"
#include "renderer/backend/Program.h"
#include "renderer/backend/Texture.h"
#include "renderer/backend/Types.h"
//#include "base/CCDirector.h"

#include <algorithm>

#ifdef CC_USE_METAL
#include "glsl_optimizer.h"
#endif

BACKEND_BEGIN

namespace {
#define MAT3_SIZE 36
#define MAT4X3_SIZE 48
#define VEC3_SIZE 12
#define VEC4_SIZE 16
#define BVEC3_SIZE 3
#define BVEC4_SIZE 4
#define IVEC3_SIZE 12
#define IVEC4_SIZE 16
    
    void convertbVec3TobVec4(const bool* src, bool* dst) {
        dst[0] = src[0];
        dst[1] = src[1];
        dst[2] = src[2];
        dst[3] = false;
    }
    
    void convertiVec3ToiVec4(const int* src, int* dst) {
        dst[0] = src[0];
        dst[1] = src[1];
        dst[2] = src[2];
        dst[3] = 0;
    }
    
    void convertVec3ToVec4(const float* src, float* dst) {
        dst[0] = src[0];
        dst[1] = src[1];
        dst[2] = src[2];
        dst[3] = 0.0f;
    }
    
    void convertMat3ToMat4x3(const float* src, float* dst) {
        dst[3] = dst[7] = dst[11] = 0.0f;
        dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
        dst[4] = src[3]; dst[5] = src[4]; dst[6] = src[5];
        dst[8] = src[6]; dst[9] = src[7]; dst[10] = src[8];
    }
}

std::vector<ProgramState::AutoBindingResolver*> ProgramState::custom_auto_binding_resolvers;

TextureInfo::TextureInfo(const std::vector<uint32_t>& slots, const std::vector<backend::TextureBackend*> textures)
: slot_(slots)
, textures_(textures) {
    retainTextures();
}

TextureInfo::TextureInfo(const TextureInfo &other)
    : slot_(other.slot_)
    , textures_(other.textures_) {
    retainTextures();
}

TextureInfo::~TextureInfo() {
    releaseTextures();
}

void TextureInfo::retainTextures() {
    for (auto& texture : textures_) {
        SAFE_RETAIN(texture);
    }
}

void TextureInfo::releaseTextures() {
    for (auto& texture : textures_) {
        SAFE_RELEASE(texture);
    }
}

TextureInfo& TextureInfo::operator=(TextureInfo&& rhs) {
    if (this != &rhs) {
        slot_ = rhs.slot_;
        
        rhs.retainTextures();
        releaseTextures();
        textures_ = rhs.textures_;
        
        rhs.releaseTextures();
        rhs.textures_.clear();
    }
    return *this;
}

TextureInfo& TextureInfo::operator=(const TextureInfo& rhs) {
    if (this != &rhs) {
        slot_ = rhs.slot_;
        textures_ = rhs.textures_;
        retainTextures();
    }
    return *this;
}

ProgramState::ProgramState(Program* program) {
    init(program);
}

bool ProgramState::init(Program* program) {
    SAFE_RETAIN(program);
    program_ = program;
    vertex_uniform_buffer_size = program_->getUniformBufferSize(ShaderStage::VERTEX);
    vertex_uniform_buffer = new char[vertex_uniform_buffer_size];
    memset(vertex_uniform_buffer, 0, vertex_uniform_buffer_size);
#ifdef USE_METAL
    fragment_uniform_buffer_size = program_->getUniformBufferSize(ShaderStage::FRAGMENT);
    fragment_uniform_buffer = new char[fragment_uniform_buffer_size];
    memset(fragment_uniform_buffer, 0, fragment_uniform_buffer_size);
#endif

    return true;
}

void ProgramState::resetUniforms() {}

ProgramState::ProgramState() {}

ProgramState::~ProgramState() {
    SAFE_RELEASE(program_);
    SAFE_DELETE_ARRAY(vertex_uniform_buffer);
    SAFE_DELETE_ARRAY(fragment_uniform_buffer);
}

ProgramState *ProgramState::clone() const {
    ProgramState *cp = new ProgramState();
    cp->program_ = program_;
    cp->vertex_uniform_buffer_size = vertex_uniform_buffer_size;
    cp->fragment_uniform_buffer_size = fragment_uniform_buffer_size;
    cp->vertex_texture_infos = vertex_texture_infos;
    cp->fragment_texture_infos = fragment_texture_infos;
    cp->vertex_uniform_buffer = new char[vertex_uniform_buffer_size];
    memcpy(cp->vertex_uniform_buffer, vertex_uniform_buffer, vertex_uniform_buffer_size);
    cp->vertex_layout = vertex_layout;
#ifdef USE_METAL
    cp->fragment_uniform_buffer = new char[fragment_uniform_buffer_size];
    memcpy(cp->fragment_uniform_buffer, fragment_uniform_buffer, fragment_uniform_buffer_size);
#endif
    SAFE_RETAIN(cp->program_);

    return cp;
}

backend::UniformLocation ProgramState::getUniformLocation(backend::Uniform name) const
{
    return program_->getUniformLocation(name);
}

backend::UniformLocation ProgramState::getUniformLocation(const std::string& uniform) const
{
    return program_->getUniformLocation(uniform);
}

void ProgramState::setCallbackUniform(const backend::UniformLocation& uniformLocation,const UniformCallback& callback) {
    callback_uniforms[uniformLocation] = callback;
}

void ProgramState::setUniform(const backend::UniformLocation& uniformLocation, const void* data, std::size_t size) {
    switch (uniformLocation.shader_stage) {
        case backend::ShaderStage::VERTEX:
            setVertexUniform(uniformLocation.location_[0], data, size, uniformLocation.location_[1]);
            break;
        case backend::ShaderStage::FRAGMENT:
            setFragmentUniform(uniformLocation.location_[1], data, size);
            break;
        case backend::ShaderStage::VERTEX_AND_FRAGMENT:
            setVertexUniform(uniformLocation.location_[0], data, size, uniformLocation.location_[1]);
            setFragmentUniform(uniformLocation.location_[1], data, size);
            break;
        default:
            break;
    }
}

#ifdef USE_METAL
void ProgramState::convertAndCopyUniformData(const backend::UniformInfo& uniformInfo, const void* srcData, std::size_t srcSize, void* buffer)
{
    auto basicType = static_cast<glslopt_basic_type>(uniformInfo.type);
    char* convertedData = new char[uniformInfo.size];
    memset(convertedData, 0, uniformInfo.size);
    int offset = 0;
    switch (basicType) {
        case kGlslTypeFloat: {
            if(uniformInfo.isMatrix) {
                for (int i=0; i<uniformInfo.count; i++) {
                    if(offset >= srcSize)
                        break;
                    
                    convertMat3ToMat4x3((float*)srcData + offset, (float*)convertedData + i * MAT4X3_SIZE);
                    offset += MAT3_SIZE;
                }
            } else {
                for (int i=0; i<uniformInfo.count; i++) {
                    if(offset >= srcSize)
                        break;
                    
                    convertVec3ToVec4((float*)srcData +offset, (float*)convertedData + i * VEC4_SIZE);
                    offset += VEC3_SIZE;
                }
            }
            break;
        }
        case kGlslTypeBool:
        {
            for (int i=0; i<uniformInfo.count; i++)
            {
                if(offset >= srcSize)
                    break;
                
                convertbVec3TobVec4((bool*)srcData + offset, (bool*)convertedData + i * BVEC4_SIZE);
                offset += BVEC3_SIZE;
            }
            break;
        }
        case kGlslTypeInt:
        {
            for (int i=0; i<uniformInfo.count; i++)
            {
                if(offset >= srcSize)
                    break;
                
                convertiVec3ToiVec4((int*)srcData + offset, (int*)convertedData + i * IVEC4_SIZE);
                offset += IVEC3_SIZE;
            }
            break;
        }
        default:
            CC_ASSERT(false);
            break;
    }
    
    memcpy((char*)buffer + uniformInfo.location, convertedData, uniformInfo.size);
    CC_SAFE_DELETE_ARRAY(convertedData);
}
#endif

void ProgramState::setVertexUniform(int location, const void* data, std::size_t size, std::size_t offset)
{
    if(location < 0)
        return;
    
//float3 etc in Metal has both sizeof and alignment same as float4, need convert to correct laytout
#ifdef CC_USE_METAL
    const auto& uniformInfo = program_->getActiveUniformInfo(ShaderStage::VERTEX, location);
    if(uniformInfo.needConvert)
    {
        convertAndCopyUniformData(uniformInfo, data, size, vertex_uniform_buffer);
    }
    else
    {
        memcpy(vertex_uniform_buffer + location, data, size);
    }
#else
    memcpy(vertex_uniform_buffer + offset, data, size);
#endif
}

void ProgramState::setFragmentUniform(int location, const void* data, std::size_t size)
{
    if(location < 0)
        return;
   
//float3 etc in Metal has both sizeof and alignment same as float4, need convert to correct laytout
#ifdef CC_USE_METAL
    const auto& uniformInfo = program_->getActiveUniformInfo(ShaderStage::FRAGMENT, location);
    if(uniformInfo.needConvert)
    {
        convertAndCopyUniformData(uniformInfo, data, size, fragment_uniform_buffer);
    }
    else
    {
        memcpy(fragment_uniform_buffer + location, data, size);
    }
#endif
}

void ProgramState::setTexture(const backend::UniformLocation& uniformLocation, uint32_t slot, backend::TextureBackend* texture) {
    switch (uniformLocation.shader_stage) {
        case backend::ShaderStage::VERTEX:
            setTexture(uniformLocation.location_[0], slot, texture, vertex_texture_infos);
            break;
        case backend::ShaderStage::FRAGMENT:
            setTexture(uniformLocation.location_[1], slot, texture, fragment_texture_infos);
            break;
        case backend::ShaderStage::VERTEX_AND_FRAGMENT:
            setTexture(uniformLocation.location_[0], slot, texture, vertex_texture_infos);
            setTexture(uniformLocation.location_[1], slot, texture, fragment_texture_infos);
            break;
        default:
            break;
    }
}

void ProgramState::setTextureArray(const backend::UniformLocation& uniformLocation, const std::vector<uint32_t>& slots, const std::vector<backend::TextureBackend*> textures) {
    switch (uniformLocation.shader_stage) {
        case backend::ShaderStage::VERTEX:
            setTextureArray(uniformLocation.location_[0], slots, textures, vertex_texture_infos);
            break;
        case backend::ShaderStage::FRAGMENT:
            setTextureArray(uniformLocation.location_[1], slots, textures, fragment_texture_infos);
            break;
        case backend::ShaderStage::VERTEX_AND_FRAGMENT:
            setTextureArray(uniformLocation.location_[0], slots, textures, vertex_texture_infos);
            setTextureArray(uniformLocation.location_[1], slots, textures, fragment_texture_infos);
            break;
        default:
            break;
    }
}

void ProgramState::setTexture(int location, uint32_t slot, backend::TextureBackend* texture, std::unordered_map<int, TextureInfo>& textureInfo) {
    if(location < 0)
        return;
    TextureInfo& info = textureInfo[location];
    info.releaseTextures();
    info.slot_ = {slot};
    info.textures_ = {texture};
    info.retainTextures();
}

void ProgramState::setTextureArray(int location, const std::vector<uint32_t>& slots, const std::vector<backend::TextureBackend*> textures, std::unordered_map<int, TextureInfo>& textureInfo) {
    assert(slots.size() == textures.size());
    TextureInfo& info = textureInfo[location];
    info.releaseTextures();
    info.slot_ = slots;
    info.textures_ = textures;
    info.retainTextures();
}

void ProgramState::setParameterAutoBinding(const std::string &uniform, const std::string &autoBinding) {
    auto_bindings.emplace(uniform, autoBinding);
    applyAutoBinding(uniform, autoBinding);
}

void ProgramState::applyAutoBinding(const std::string &uniformName, const std::string &autoBinding) {
    bool resolved = false;
    for (const auto resolver : custom_auto_binding_resolvers) {
        resolved = resolver->resolveAutoBinding(this, uniformName, autoBinding);
        if (resolved) break;
    }
}

ProgramState::AutoBindingResolver::AutoBindingResolver() {
    custom_auto_binding_resolvers.emplace_back(this);
}

ProgramState::AutoBindingResolver::~AutoBindingResolver() {
    auto &list = custom_auto_binding_resolvers;
    list.erase(std::remove(list.begin(), list.end(), this), list.end());
}

void ProgramState::getVertexUniformBuffer(char** buffer, std::size_t& size) const {
    *buffer = vertex_uniform_buffer;
    size = vertex_uniform_buffer_size;
}

void ProgramState::getFragmentUniformBuffer(char** buffer, std::size_t& size) const {
    *buffer = fragment_uniform_buffer;
    size = fragment_uniform_buffer_size;
}

BACKEND_END

