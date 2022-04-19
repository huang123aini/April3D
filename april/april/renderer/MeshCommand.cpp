//
//  MeshCommand.cpp
//  april3d
//
//  Created by 黄世平 on 2022/4/4.
//

#include "MeshCommand.h"

#include "renderer/MeshCommand.h"
#include "base/aprilMacros.h"
#include "base/CCConfiguration.h"
#include "base/CCDirector.h"
#include "2d/CCLight.h"
#include "2d/CCCamera.h"
#include "renderer/CCRenderer.h"
#include "renderer/CCTextureAtlas.h"
#include "renderer/CCTexture2D.h"
#include "renderer/CCTechnique.h"
#include "renderer/CCMaterial.h"
#include "renderer/CCPass.h"
#include "xxhash.h"

APRIL_BEGIN


MeshCommand::MeshCommand() {
    type_ = RenderCommand::Type::MESH_COMMAND;
    is_3D = true;
}

void MeshCommand::init(float globalZOrder) {
    CustomCommand::init(globalZOrder);
}

void MeshCommand::init(float globalZOrder, const Mat4 &transform) {
    CustomCommand::init(globalZOrder);
    if (Camera::getVisitingCamera()) {
        depth_ = Camera::getVisitingCamera()->getDepthInView(transform);
    }
    mv_matrix = transform;
}

MeshCommand::~MeshCommand() { }

APRIL_END
