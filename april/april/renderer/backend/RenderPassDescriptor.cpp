//
//  RenderPassDescriptor.cpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/2.
//

#include "RenderPassDescriptor.h"

BACKEND_BEGIN

RenderPassDescriptor& RenderPassDescriptor::operator=(const RenderPassDescriptor& descriptor) {
    clear_depth_value = descriptor.clear_depth_value;
    clear_stencil_value = descriptor.clear_stencil_value;
    clear_color_value = descriptor.clear_color_value;
    need_color_attachment = descriptor.need_color_attachment;
    depth_test_enabled = descriptor.depth_test_enabled;
    stencil_test_enabled = descriptor.stencil_test_enabled;
    need_clear_color = descriptor.need_clear_color;
    need_clear_depth = descriptor.need_clear_depth;
    need_clear_stencil = descriptor.need_clear_stencil;
    depth_attachment_texture = descriptor.depth_attachment_texture;
    stencil_attachment_texture = descriptor.stencil_attachment_texture;
    color_attachments_texture[0] = descriptor.color_attachments_texture[0];
    return *this;
}

bool RenderPassDescriptor::operator==(const RenderPassDescriptor& descriptor) const {
    if( clear_depth_value == descriptor.clear_depth_value &&
       clear_stencil_value == descriptor.clear_stencil_value &&
       clear_color_value == descriptor.clear_color_value &&
       need_color_attachment == descriptor.need_color_attachment &&
       depth_test_enabled == descriptor.depth_test_enabled &&
       stencil_test_enabled == descriptor.stencil_test_enabled &&
       need_clear_color == descriptor.need_clear_color &&
       need_clear_depth == descriptor.need_clear_depth &&
       need_clear_stencil == descriptor.need_clear_stencil &&
       depth_attachment_texture == descriptor.depth_attachment_texture &&
       stencil_attachment_texture == descriptor.stencil_attachment_texture &&
       color_attachments_texture[0] == descriptor.color_attachments_texture[0]) {
        return true;
    } else {
        return false;
    }
}

BACKEND_END
