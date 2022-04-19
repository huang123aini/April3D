//
//  ProgramGL.cpp
//  april3d
//
//  Created by 黄世平 on 2022/4/3.
//

#include "ProgramGL.h"
#include "ShaderModuleGL.h"
#include "renderer/backend/Types.h"
#include "base/aprilMacros.h"
#include "renderer/backend/opengl/UtilsGL.h"

#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>

BACKEND_BEGIN
namespace {
    std::string vsPreDefine("#version 100\n precision highp float;\n precision highp int;\n");
    std::string fsPreDefine("precision mediump float;\n precision mediump int;\n");
}

ProgramGL::ProgramGL(const std::string& vertexShader, const std::string& fragmentShader)
: Program(vertexShader, fragmentShader)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    //some device required manually specify the precision qualifiers for vertex shader.
    vertex_shader_module = static_cast<ShaderModuleGL*>(ShaderCache::newVertexShaderModule(std::move(vsPreDefine + vertex_shader)));
    fragment_shader_module = static_cast<ShaderModuleGL*>(ShaderCache::newFragmentShaderModule(std::move(fsPreDefine +  fragment_shader)));
#else
    vertex_shader_module = static_cast<ShaderModuleGL*>(ShaderCache::newVertexShaderModule(vertex_shader));
    fragment_shader_module = static_cast<ShaderModuleGL*>(ShaderCache::newFragmentShaderModule(fragment_shader));
#endif

    SAFE_RETAIN(vertex_shader_module);
    SAFE_RETAIN(fragment_shader_module);
    compileProgram();
    computeUniformInfos();
    computeLocations();
}

ProgramGL::~ProgramGL() {
    SAFE_RELEASE(vertex_shader_module);
    SAFE_RELEASE(fragment_shader_module);
    if (program_)
        glDeleteProgram(program_);

#if CC_ENABLE_CACHE_TEXTURE_DATA
    Director::getInstance()->getEventDispatcher()->removeEventListener(_backToForegroundListener);
#endif
}

void ProgramGL::compileProgram() {
    if (vertex_shader_module == nullptr || fragment_shader_module == nullptr) {
        return;
    }

    auto vertShader = vertex_shader_module->getShader();
    auto fragShader = fragment_shader_module->getShader();
    
    assert (vertShader != 0 && fragShader != 0);
    if (vertShader == 0 || fragShader == 0) {
        return;
    }
    program_ = glCreateProgram();
    if (!program_)
        return;
    
    glAttachShader(program_, vertShader);
    glAttachShader(program_, fragShader);
    
    glLinkProgram(program_);
    
    GLint status = 0;
    glGetProgramiv(program_, GL_LINK_STATUS, &status);
    if (GL_FALSE == status)
    {
        printf("cocos2d: ERROR: %s: failed to link program ", __FUNCTION__);
        glDeleteProgram(program_);
        program_ = 0;
    }
}

void ProgramGL::computeLocations() {
    std::fill(built_in_attribute_location, built_in_attribute_location + ATTRIBUTE_MAX, -1);
//    std::fill(built_in_uniform_location, built_in_uniform_location + UNIFORM_MAX, -1);

    ///a_position
    auto location = glGetAttribLocation(program_, ATTRIBUTE_NAME_POSITION);
    built_in_attribute_location[Attribute::POSITION] = location;

    ///a_color
    location = glGetAttribLocation(program_, ATTRIBUTE_NAME_COLOR);
    built_in_attribute_location[Attribute::COLOR] = location;

    ///a_texCoord
    location = glGetAttribLocation(program_, ATTRIBUTE_NAME_TEXCOORD);
    built_in_attribute_location[Attribute::TEXCOORD0] = location;

    ///u_MVPMatrix
    location = glGetUniformLocation(program_, UNIFORM_NAME_MVP_MATRIX);
    built_in_uniform_location[Uniform::MVP_MATRIX].location_[0] = location;
    built_in_uniform_location[Uniform::MVP_MATRIX].location_[1] = active_uniform_infos[UNIFORM_NAME_MVP_MATRIX].buffer_offset;

    ///u_textColor
    location = glGetUniformLocation(program_, UNIFORM_NAME_TEXT_COLOR);
    built_in_uniform_location[Uniform::TEXTURE_COLOR].location_[0] = location;
    built_in_uniform_location[Uniform::TEXTURE_COLOR].location_[1] = active_uniform_infos[UNIFORM_NAME_TEXT_COLOR].buffer_offset;

    ///u_effectColor
    location = glGetUniformLocation(program_, UNIFORM_NAME_EFFECT_COLOR);
    built_in_uniform_location[Uniform::EFFECT_COLOR].location_[0] = location;
    built_in_uniform_location[Uniform::EFFECT_COLOR].location_[1] = active_uniform_infos[UNIFORM_NAME_EFFECT_COLOR].buffer_offset;

    ///u_effectType
    location = glGetUniformLocation(program_, UNIFORM_NAME_EFFECT_TYPE);
    built_in_uniform_location[Uniform::EFFECT_TYPE].location_[0] = location;
    built_in_uniform_location[Uniform::EFFECT_TYPE].location_[1] = active_uniform_infos[UNIFORM_NAME_EFFECT_TYPE].buffer_offset;

    ///u_texture
    location = glGetUniformLocation(program_, UNIFORM_NAME_TEXTURE);
    built_in_uniform_location[Uniform::TEXTURE0].location_[0] = location;

    ///u_texture1
    location = glGetUniformLocation(program_, UNIFORM_NAME_TEXTURE1);
    built_in_uniform_location[Uniform::TEXTURE1].location_[0] = location;
}

bool ProgramGL::getAttributeLocation(const std::string& attributeName, unsigned int& location) const {
    GLint loc = glGetAttribLocation(program_, attributeName.c_str());
    if (-1 == loc) {
        printf("april3d: %s: can not find vertex attribute of %s", __FUNCTION__, attributeName.c_str());
        return false;
    }
    
    location = GLuint(loc);
    return true;
}

const std::unordered_map<std::string, AttributeBindInfo> ProgramGL::getActiveAttributes() const {
    std::unordered_map<std::string, AttributeBindInfo> attributes;

    if (!program_) {
        return attributes;
    }

    GLint numOfActiveAttributes = 0;
    glGetProgramiv(program_, GL_ACTIVE_ATTRIBUTES, &numOfActiveAttributes);


    if (numOfActiveAttributes <= 0) {
        return attributes;
    }
        
    attributes.reserve(numOfActiveAttributes);

    int MAX_ATTRIBUTE_NAME_LENGTH = 256;
    std::vector<char> attrName(MAX_ATTRIBUTE_NAME_LENGTH + 1);

    GLint attrNameLen = 0;
    GLenum attrType;
    GLint attrSize;
    backend::AttributeBindInfo info;

    for (int i = 0; i < numOfActiveAttributes; i++) {
        glGetActiveAttrib(program_, i, MAX_ATTRIBUTE_NAME_LENGTH, &attrNameLen, &attrSize, &attrType, attrName.data());
        CHECK_GL_ERROR_DEBUG();
        info.attribute_name = std::string(attrName.data(), attrName.data() + attrNameLen);
        info.location_ = glGetAttribLocation(program_, info.attribute_name.c_str());
        info.type_ = attrType;
        info.size_ = UtilsGL::getGLDataTypeSize(attrType) * attrSize;
        CHECK_GL_ERROR_DEBUG();
        attributes[info.attribute_name] = info;
    }

    return attributes;

}

void ProgramGL::computeUniformInfos()
{
    if (!program_)
    return;
    
    GLint numOfUniforms = 0;
    glGetProgramiv(program_, GL_ACTIVE_UNIFORMS, &numOfUniforms);
    if (!numOfUniforms)
    return;
    
#define MAX_UNIFORM_NAME_LENGTH 256
    UniformInfo uniform;
    GLint length = 0;
    total_buffer_size = 0;
    max_location = -1;
    active_uniform_infos.clear();
    GLchar* uniformName = (GLchar*)malloc(MAX_UNIFORM_NAME_LENGTH + 1);
    for (int i = 0; i < numOfUniforms; ++i) {
        glGetActiveUniform(program_, i, MAX_UNIFORM_NAME_LENGTH, &length, &uniform.count_, &uniform.type_, uniformName);
        uniformName[length] = '\0';
        
        if (length > 3) {
            char* c = strrchr(uniformName, '[');
            if (c) {
                *c = '\0';
                uniform.is_array = true;
            }
        }
        uniform.location_ = glGetUniformLocation(program_, uniformName);
        uniform.size_ = UtilsGL::getGLDataTypeSize(uniform.type_);
        uniform.buffer_offset = (uniform.size_ == 0) ? 0 : static_cast<unsigned int>(total_buffer_size);
        active_uniform_infos[uniformName] = uniform;
        total_buffer_size += uniform.size_ * uniform.count_;
        max_location = max_location <= uniform.location_ ? (uniform.location_ + 1) : max_location;
    }
    free(uniformName);
}

int ProgramGL::getAttributeLocation(Attribute name) const {
    return built_in_attribute_location[name];
}

int ProgramGL::getAttributeLocation(const std::string& name) const {
    return glGetAttribLocation(program_, name.c_str());
}

UniformLocation ProgramGL::getUniformLocation(backend::Uniform name) const {
   return built_in_uniform_location[name];
}

UniformLocation ProgramGL::getUniformLocation(const std::string& uniform) const {
    UniformLocation uniformLocation;
    if (active_uniform_infos.find(uniform) != active_uniform_infos.end()) {
        const auto &uniformInfo = active_uniform_infos.at(uniform);
        uniformLocation.location_[0] = uniformInfo.location_;
        uniformLocation.location_[1] = uniformInfo.buffer_offset;
    }
    return uniformLocation;
}

int ProgramGL::getMaxVertexLocation() const {
    return max_location;
}
int ProgramGL::getMaxFragmentLocation() const {
    return max_location;
}

const UniformInfo& ProgramGL::getActiveUniformInfo(ShaderStage stage, int location) const {
    return std::move(UniformInfo{});
}

const std::unordered_map<std::string, UniformInfo>& ProgramGL::getAllActiveUniformInfo(ShaderStage stage) const {
    return active_uniform_infos;
}

std::size_t ProgramGL::getUniformBufferSize(ShaderStage stage) const {
    return total_buffer_size;
}

BACKEND_END
