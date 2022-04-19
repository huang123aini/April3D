//
//  VertexLayout.cpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/2.
//

#include "VertexLayout.h"
#include "base/aprilMacros.h"
#include <cassert>

BACKEND_BEGIN

void VertexLayout::setAttribute(const std::string &name, std::size_t index, VertexFormat format, std::size_t offset, bool needToBeNormallized) {
    if(index == -1) {
        return;
    }
    attributes_[name] = { name, index, format, offset, needToBeNormallized };
}

void VertexLayout::setLayout(std::size_t stride) {
    stride_ = stride;
}

BACKEND_END
