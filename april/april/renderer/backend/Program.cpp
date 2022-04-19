//
//  Program.cpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/2.
//

#include "Program.h"
#include "ProgramCache.h"

BACKEND_BEGIN

Program::Program(const std::string& vs, const std::string& fs)
: vertex_shader(vs)
, fragment_shader(fs) {}

void Program::setProgramType(ProgramType type) {
    program_type = type;
}

Program* Program::getBuiltinProgram(ProgramType type) {
    return ProgramCache::getInstance()->getBuiltinProgram(type);
}

BACKEND_END
