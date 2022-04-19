#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <cstdint>
#include <functional>
#include "platform/AprilPlatformMacros.h"
#include "base/Ref.h"
#include "renderer/backend/Types.h"
#include "renderer/backend/Program.h"
#include "renderer/backend/VertexLayout.h"

BACKEND_BEGIN

class TextureBackend;
class VertexLayout;

 /**
  *保存纹理信息
  */
struct TextureInfo {
    TextureInfo(const std::vector<uint32_t>& _slots, const std::vector<backend::TextureBackend*> _textures);
    TextureInfo() = default;
    TextureInfo(const TextureInfo &);
    ~TextureInfo();
    TextureInfo& operator=(TextureInfo&& rhs);
    TextureInfo& operator=(const TextureInfo& rhs);

    void retainTextures();
    void releaseTextures();

    std::vector<uint32_t> slot_;
    std::vector<backend::TextureBackend*> textures_;
#if ENABLE_CACHE_TEXTURE_DATA
    int location_ = -1;
#endif
};

class ProgramState : public april3d::Ref
{
public:
    using UniformCallback = std::function<void(ProgramState*, const UniformLocation &)>;

    ProgramState(Program* program);
    
    virtual ~ProgramState();
    
    ProgramState *clone() const;
    
    backend::Program* getProgram() const { return program_; }
    
    void setUniform(const backend::UniformLocation& uniformLocation, const void* data, std::size_t size);

    backend::UniformLocation getUniformLocation(const std::string& uniform) const;

    backend::UniformLocation getUniformLocation(backend::Uniform name) const;

    inline int getAttributeLocation(const std::string& name) const { return program_->getAttributeLocation(name); }

    inline int getAttributeLocation(Attribute name) const { return program_->getAttributeLocation(name); }

    void setCallbackUniform(const backend::UniformLocation&, const UniformCallback &);

    void setTexture(const backend::UniformLocation& uniformLocation, uint32_t slot, backend::TextureBackend* texture);

    void setTextureArray(const backend::UniformLocation& uniformLocation, const std::vector<uint32_t>& slots, const std::vector<backend::TextureBackend*> textures);

    inline const std::unordered_map<int, TextureInfo>& getVertexTextureInfos() const { return vertex_texture_infos; }

    inline const std::unordered_map<int, TextureInfo>& getFragmentTextureInfos() const { return fragment_texture_infos; }

    inline const std::unordered_map<UniformLocation, UniformCallback, UniformLocation>& getCallbackUniforms() const { return callback_uniforms; }

    void getVertexUniformBuffer(char** buffer, std::size_t& size) const;

    void getFragmentUniformBuffer(char** buffer, std::size_t& size) const;
    
    class  AutoBindingResolver {
    public:
        AutoBindingResolver();
        virtual ~AutoBindingResolver();
     
        virtual bool resolveAutoBinding(ProgramState *, const std::string &uniformName, const std::string &autoBinding) = 0;
    };
  
    void setParameterAutoBinding(const std::string &uniformName, const std::string &autoBinding);

    inline std::shared_ptr<VertexLayout> getVertexLayout() const { return vertex_layout; }
protected:

    ProgramState();

    void setVertexUniform(int location, const void* data, std::size_t size, std::size_t offset);

    void setFragmentUniform(int location, const void* data, std::size_t size);

    void setTexture(int location, uint32_t slot, backend::TextureBackend* texture, std::unordered_map<int, TextureInfo>& textureInfo);
    
    void setTextureArray(int location, const std::vector<uint32_t>& slots, const std::vector<backend::TextureBackend*> textures, std::unordered_map<int, TextureInfo>& textureInfo);
    
    void resetUniforms();

    bool init(Program* program);
    
#ifdef USE_METAL
  
    void convertAndCopyUniformData(const backend::UniformInfo& uniformInfo, const void* srcData, std::size_t srcSize, void* buffer);
#endif
  
    void applyAutoBinding(const std::string &, const std::string &);

    backend::Program*                                       program_ = nullptr;
    std::unordered_map<UniformLocation, UniformCallback, UniformLocation>   callback_uniforms;
    char* vertex_uniform_buffer = nullptr;
    char* fragment_uniform_buffer = nullptr;
    std::size_t vertex_uniform_buffer_size = 0;
    std::size_t fragment_uniform_buffer_size = 0;

    std::unordered_map<int, TextureInfo>                    vertex_texture_infos;
    std::unordered_map<int, TextureInfo>                    fragment_texture_infos;

    std::unordered_map<std::string, std::string>            auto_bindings;

    static std::vector<AutoBindingResolver*>                custom_auto_binding_resolvers;
    std::shared_ptr<VertexLayout> vertex_layout = std::make_shared<VertexLayout>();

};

BACKEND_END
