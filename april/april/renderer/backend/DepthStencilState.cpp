//
//  DepthStencilState.cpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/2.
//

#include "DepthStencilState.h"

BACKEND_BEGIN

bool StencilDescriptor::operator==(const StencilDescriptor &rhs) const {
    return (stencil_failure_operation == rhs.stencil_failure_operation &&
            depth_failure_operation == rhs.depth_failure_operation &&
            depth_stencil_pass_operation == rhs.depth_stencil_pass_operation &&
            stencil_compare_function == rhs.stencil_compare_function &&
            read_mask == rhs.read_mask &&
            write_mask == rhs.write_mask);
}

DepthStencilState::DepthStencilState(const DepthStencilDescriptor& descriptor)
: depth_stencil_descriptor(descriptor) {
    is_back_front_stencil_equal = descriptor.back_face_stencil == descriptor.front_face_stencil;
}

DepthStencilState::~DepthStencilState() {}

BACKEND_END
