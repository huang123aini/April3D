//
//  RenderPassDescriptor.hpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/2.
//

#ifndef RenderPassDescriptor_hpp
#define RenderPassDescriptor_hpp

#include <stdio.h>
#include <array>
#include <vector>

#include "Macros.h"
#include "Types.h"

BACKEND_BEGIN

class TextureBackend;

/**
 * Store values about color, depth and stencil attachment.
 */
struct RenderPassDescriptor {
    RenderPassDescriptor& operator=(const RenderPassDescriptor& descriptor);
    bool operator==(const RenderPassDescriptor& descriptor) const;
    bool needDepthStencilAttachment() const { return depth_test_enabled || stencil_test_enabled; }

    float clear_depth_value = 0.f;
    float clear_stencil_value = 0.f;
    std::array<float, 4> clear_color_value {{0.f, 0.f, 0.f, 0.f}};
    bool need_color_attachment = true;
    bool depth_test_enabled = false;
    bool stencil_test_enabled = false;
    bool need_clear_color = false;
    bool need_clear_depth = false;
    bool need_clear_stencil = false;
    /**
     *深度纹理
     */
    TextureBackend* depth_attachment_texture = nullptr;
    /**
     *模板纹理
     */
    TextureBackend* stencil_attachment_texture = nullptr;
    /**
     *颜色纹理
     */
    TextureBackend* color_attachments_texture[MAX_COLOR_ATTCHMENT] = { nullptr };
};

BACKEND_END


#endif /* RenderPassDescriptor_hpp */
