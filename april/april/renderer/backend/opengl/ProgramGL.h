//
//  ProgramGL.hpp
//  april3d
//
//  Created by 黄世平 on 2022/4/3.
//

#ifndef ProgramGL_hpp
#define ProgramGL_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <unordered_map>

#include "../Macros.h"
#include "../Types.h"
#include "../RenderPipelineDescriptor.h"
#include "base/Ref.h"
#include "platform/AprilGL.h"
#include "../Program.h"



BACKEND_BEGIN

class ShaderModuleGL;

struct AttributeInfo {
    unsigned int location_ = 0;
    unsigned int size_ = 0;
    GLenum type_ = GL_BYTE;
    GLsizei stride_ = 0;
    unsigned int offset_ = 0;
    GLboolean need_to_be_normallized = GL_FALSE;
    std::string name_;
};

class ProgramGL : public Program
{
public:
   
    ProgramGL(const std::string& vertexShader, const std::string& fragmentShader);

    ~ProgramGL();
    
    inline const std::vector<AttributeInfo>& getAttributeInfos() const { return attribute_infos; }

    inline GLuint getHandler() const { return program_; }

    /**
     * Calculate attribute information according to vertex layout.
     */
    void computeAttributeInfos(const RenderPipelineDescriptor& descriptor);

    /**
     * Get uniform location by name.
     * @param uniform Specifies the uniform name.
     * @return The uniform location.
     */
    virtual UniformLocation getUniformLocation(const std::string& uniform) const override;

    /**
     * Get uniform location by engine built-in uniform enum name.
     * @param name Specifies the engine built-in uniform enum name.
     * @return The uniform location.
     */
    virtual UniformLocation getUniformLocation(backend::Uniform name) const override;

    /**
     * Get attribute location by attribute name.
     * @param name Specifies the attribute name.
     * @return The attribute location.
     */
    virtual int getAttributeLocation(const std::string& name) const override;

    /**
     * Get attribute location by engine built-in attribute enum name.
     * @param name Specifies the engine built-in attribute enum name.
     * @return The attribute location.
     */
    virtual int getAttributeLocation(Attribute name) const override;

    /**
     * Get maximum vertex location.
     * @return Maximum vertex locaiton.
     */
    virtual int getMaxVertexLocation() const override;

    /**
     * Get maximum fragment location.
     * @return Maximum fragment location.
     */
    virtual int getMaxFragmentLocation() const override;

    /**
     * Get active vertex attributes.
     * @return Active vertex attributes. key is active attribute name, Value is corresponding attribute info.
     */
    virtual const std::unordered_map<std::string, AttributeBindInfo> getActiveAttributes() const override;
    
    /**
     * Get uniform buffer size in bytes that can hold all the uniforms.
     * @param stage Specifies the shader stage. The symbolic constant can be either VERTEX or FRAGMENT.
     * @return The uniform buffer size in bytes.
     */
    virtual std::size_t getUniformBufferSize(ShaderStage stage) const override;
    
    /**
     * Get a uniformInfo in given location from the specific shader stage.
     * @param stage Specifies the shader stage. The symbolic constant can be either VERTEX or FRAGMENT.
     * @param location Specifies the uniform locaion.
     * @return The uniformInfo.
     */
    virtual const UniformInfo& getActiveUniformInfo(ShaderStage stage, int location) const override;

    /**
     * Get all uniformInfos.
     * @return The uniformInfos.
     */
    virtual const std::unordered_map<std::string, UniformInfo>& getAllActiveUniformInfo(ShaderStage stage) const override ;

private:
    void compileProgram();
    bool getAttributeLocation(const std::string& attributeName, unsigned int& location) const;
    void computeUniformInfos();
    void computeLocations();
#if ENABLE_CACHE_TEXTURE_DATA
    virtual void reloadProgram();
    virtual int getMappedLocation(int location) const override;
    virtual int getOriginalLocation(int location) const override;
    virtual const std::unordered_map<std::string, int> getAllUniformsLocation() const override { return original_uniform_locations; }
#endif
    
    GLuint program_ = 0;
    ShaderModuleGL* vertex_shader_module = nullptr;
    ShaderModuleGL* fragment_shader_module = nullptr;
    
    std::vector<AttributeInfo> attribute_infos;
    std::unordered_map<std::string, UniformInfo> active_uniform_infos;
#if ENABLE_CACHE_TEXTURE_DATA
    std::unordered_map<std::string, int> original_uniform_locations;
    std::unordered_map<int, int> map_to_current_active_location;
    std::unordered_map<int, int> map_to_original_location;
    EventListenerCustom* back_to_foreground_listener = nullptr;
#endif

    std::size_t total_buffer_size = 0;
    int max_location = -1;
    UniformLocation built_in_uniform_location[UNIFORM_MAX];
    int built_in_attribute_location[Attribute::ATTRIBUTE_MAX];
    std::unordered_map<int, int> buffer_offset;
};

BACKEND_END

#endif /* ProgramGL_hpp */
