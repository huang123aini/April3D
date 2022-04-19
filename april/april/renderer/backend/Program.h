//
//  Program.hpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/2.
//

#ifndef Program_hpp
#define Program_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <unordered_map>


#include "Macros.h"
#include "base/Ref.h"
#include "platform/AprilPlatformMacros.h"
#include "Types.h"
#include "ShaderCache.h"


BACKEND_BEGIN

class ShaderModule;

class Program : public april3d::Ref {
public:

    static Program* getBuiltinProgram(ProgramType type);
    
    virtual UniformLocation getUniformLocation(const std::string& uniform) const = 0;

    virtual UniformLocation getUniformLocation(backend::Uniform name) const = 0;

    virtual int getAttributeLocation(const std::string& name) const =  0;

    virtual int getAttributeLocation(backend::Attribute name) const =  0;
    
    virtual int getMaxVertexLocation() const = 0;

    virtual int getMaxFragmentLocation() const = 0;

    virtual const std::unordered_map<std::string, AttributeBindInfo> getActiveAttributes() const = 0;

    const std::string& getVertexShader() const { return vertex_shader; }

    const std::string& getFragmentShader() const { return fragment_shader; }
    
    ProgramType getProgramType() const { return program_type; }

    virtual std::size_t getUniformBufferSize(ShaderStage stage) const =0;

    virtual const UniformInfo& getActiveUniformInfo(ShaderStage stage, int location) const = 0;

    virtual const std::unordered_map<std::string, UniformInfo>& getAllActiveUniformInfo(ShaderStage stage) const = 0;
    
protected:
    void setProgramType(ProgramType type);

    Program(const std::string& vs, const std::string& fs);

    friend class ProgramCache;
    
    std::string vertex_shader;
    std::string fragment_shader;
    ProgramType program_type = ProgramType::CUSTOM_PROGRAM;
};

BACKEND_END

#endif /* Program_hpp */
