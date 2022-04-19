//
//  ShaderModuleGL.cpp
//  april3d
//
//  Created by 黄世平 on 2022/4/3.
//

#include "ShaderModuleGL.h"

#include "platform/AprilPlatformMacros.h"
#include "base/aprilMacros.h"


BACKEND_BEGIN

ShaderModuleGL::ShaderModuleGL(ShaderStage stage, const std::string& source)
: ShaderModule(stage) {
    compileShader(stage, source);
}

ShaderModuleGL::~ShaderModuleGL() {
    deleteShader();
}

void ShaderModuleGL::compileShader(ShaderStage stage, const std::string &source) {
    GLenum shaderType = stage == ShaderStage::VERTEX ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;
    const GLchar* sourcePtr = reinterpret_cast<const GLchar*>(source.c_str());
    shader_ = glCreateShader(shaderType);
    if (!shader_) {
        return;
    }
    glShaderSource(shader_, 1, &sourcePtr, nullptr);
    glCompileShader(shader_);
    
    GLint status = 0;
    glGetShaderiv(shader_, GL_COMPILE_STATUS, &status);
    if (!status) {
        printf("april: ERROR: Failed to compile shader:\n%s. \n", source.c_str());
        printf("april: %s \n", getErrorLog(shader_));
        deleteShader();
        printf("Shader Compile Failed. \n");
    }
}

char* ShaderModuleGL::getErrorLog(GLuint shader) const {
    GLint logLength = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
    char* log = (char*)malloc(sizeof(char) * logLength);
    glGetShaderInfoLog(shader, logLength, nullptr, log);
    return log;
}

void ShaderModuleGL::deleteShader() {
    if (shader_) {
        glDeleteShader(shader_);
        shader_ = 0;
    }
}

BACKEND_END
