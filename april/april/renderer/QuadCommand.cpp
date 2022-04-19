//
//  QuadCommand.cpp
//  april3d
//
//  Created by 黄世平 on 2022/4/4.
//


#include "renderer/QuadCommand.h"
#include "renderer/Material.h"
#include "renderer/Technique.h"
#include "renderer/Renderer.h"
#include "renderer/Pass.h"
#include "renderer/Texture2D.h"
#include "base/ccUtils.h"

APRIL_BEGIN

int QuadCommand::index_capacity = -1;
uint16_t* QuadCommand::indices_ = nullptr;

QuadCommand::QuadCommand():
index_size(-1),
owned_indices(){}

QuadCommand::~QuadCommand() {
    for (auto& indices : owned_indices) {
        SAFE_DELETE_ARRAY(indices);
    }
}

void QuadCommand::reIndex(int indicesCount) {
    /**
     *第一次初始化:为索引创建一个合适的缓冲区大小，以防止过多的调整大小
     */
    if (index_capacity == -1) {
        indicesCount = std::max(indicesCount, 2048);
    }

    if (indicesCount > index_capacity) {
        /**
         *如果需要调整大小，请获取所需大小+ 25%，但不要大于Max大小
         */
        indicesCount *= 1.25;
        indicesCount = std::min(indicesCount, 65536);

        printf("april3d: QuadCommand: resizing index size from [%d] to [%d]", index_capacity, indicesCount);

        owned_indices.push_back(indices_);
        indices_ = new (std::nothrow) uint16_t[indicesCount];
        index_capacity = indicesCount;
    }

    for( int i=0; i < index_capacity/6; i++) {
        indices_[i*6+0] = (uint16_t) (i*4+0);
        indices_[i*6+1] = (uint16_t) (i*4+1);
        indices_[i*6+2] = (uint16_t) (i*4+2);
        indices_[i*6+3] = (uint16_t) (i*4+3);
        indices_[i*6+4] = (uint16_t) (i*4+2);
        indices_[i*6+5] = (uint16_t) (i*4+1);
    }
    index_size = indicesCount;
}

void QuadCommand::init(float globalOrder, Texture2D *texture, const BlendFunc& blendType, V3F_C4B_T2F_Quad *quads, ssize_t quadCount, const Mat4 &mv, uint32_t flags) {
    if (quadCount * 6 > index_size) {
        reIndex((int)quadCount*6);
    }
    Triangles triangles;
    triangles.verts = &quads->tl;
    triangles.vertCount = (int)quadCount * 4;
    triangles.indices = indices_;
    triangles.indexCount = (int)quadCount * 6;
    TrianglesCommand::init(globalOrder, texture, blendType, triangles, mv, flags);

    alpha_texture_id = texture->getAlphaTextureName();
}

APRIL_END
