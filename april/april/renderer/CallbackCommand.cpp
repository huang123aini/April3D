//
//  CallbackCommand.cpp
//  april3d
//
//  Created by 黄世平 on 2022/4/4.
//

#include "CallbackCommand.h"
#include "renderer/backend/Device.h"

APRIL_BEGIN

CallbackCommand::CallbackCommand() {
    type_ = RenderCommand::Type::CALLBACK_COMMAND;
}

void CallbackCommand::init(float globalOrder) {
    global_order = globalOrder;
}

void CallbackCommand::init(float globalOrder, const Mat4 &transform, unsigned int flags) {
    RenderCommand::init(globalOrder, transform, flags);
}

void CallbackCommand::execute() {
   if(callback_func) {
       callback_func();
   }
}

CaptureScreenCallbackCommand::CaptureScreenCallbackCommand() {
    type_ = RenderCommand::Type::CAPTURE_SCREEN_COMMAND;
}

void CaptureScreenCallbackCommand::init(float globalOrder) {
    Device::getInstance()->setFrameBufferOnly(false);
    global_order = globalOrder;
}

APRIL_END
