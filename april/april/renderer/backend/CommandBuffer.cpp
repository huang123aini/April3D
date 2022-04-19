//
//  CommandBuffer.cpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/2.
//

#include "CommandBuffer.h"


BACKEND_BEGIN

void CommandBuffer::setStencilReferenceValue(uint32_t value) {
    stencil_reference_value_back = stencil_reference_value_front = value;
}

void CommandBuffer::setStencilReferenceValue(unsigned int frontRef, unsigned int backRef) {
    stencil_reference_value_front = frontRef;
    stencil_reference_value_back = backRef;
}

BACKEND_END
