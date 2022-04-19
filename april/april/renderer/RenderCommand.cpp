//
//  RenderCommand.cpp
//  april3d
//
//  Created by 黄世平 on 2022/4/4.
//

#include "RenderCommand.h"

#include "renderer/RenderCommand.h"
#include "2d/CCCamera.h"
#include "2d/CCNode.h"


APRIL_BEGIN

RenderCommand::RenderCommand() {}

RenderCommand::~RenderCommand() {}

void RenderCommand::init(float globalZOrder, const cocos2d::Mat4 &transform, unsigned int flags) {
    global_order = globalZOrder;
    if (flags & Node::FLAGS_RENDER_AS_3D) {
        if (Camera::getVisitingCamera())
            depth_ = Camera::getVisitingCamera()->getDepthInView(transform);
        
        set3D(true);
    } else {
        set3D(false);
        depth_ = 0;
    }
}

void RenderCommand::printID() {
    printf("Command Depth: %f\n", global_order);
}

APRIL_END
