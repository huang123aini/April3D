//
//  ShaderModule.cpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/2.
//

#include "ShaderModule.h"

BACKEND_BEGIN

ShaderStage ShaderModule::getShaderStage() const {
    return stage_;
}

ShaderModule::ShaderModule(ShaderStage stage):
stage_(stage) {}

ShaderModule::~ShaderModule(){}

BACKEND_END
