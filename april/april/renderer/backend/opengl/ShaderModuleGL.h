//
//  ShaderModuleGL.hpp
//  april3d
//
//  Created by 黄世平 on 2022/4/3.
//

#ifndef ShaderModuleGL_hpp
#define ShaderModuleGL_hpp

#include <stdio.h>

#include "../ShaderModule.h"
#include "platform/AprilGL.h"


#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>

BACKEND_BEGIN

/**
 *创建并编译Shader
 */
class ShaderModuleGL : public ShaderModule {
public:
   
    ShaderModuleGL(ShaderStage stage, const std::string& source);
    ~ShaderModuleGL();

    inline GLuint getShader() const { return shader_; }
private:
    void compileShader(ShaderStage stage, const std::string& source);
    char* getErrorLog(GLuint shader) const;
    void deleteShader();
    
    GLuint shader_ = 0;
    friend class ProgramGL;
};

BACKEND_END

#endif /* ShaderModuleGL_hpp */
