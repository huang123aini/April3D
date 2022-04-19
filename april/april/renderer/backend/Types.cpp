//
//  Types.cpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/2.
//

#include "Types.h"

BACKEND_BEGIN


bool UniformLocation::operator==(const UniformLocation &other) const {
    return (shader_stage == other.shader_stage &&
            location_[0] == other.location_[0] &&
            location_[1] == other.location_[1]);
}

std::size_t UniformLocation::operator()(const UniformLocation &uniform) const {
    return (((size_t) shader_stage) & 0xF)|((size_t)(location_[0] << 4))|((size_t)(location_[1] << 8));
}

BACKEND_END
