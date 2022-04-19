//
//  CallbackCommand.hpp
//  april3d
//
//  Created by 黄世平 on 2022/4/4.
//

#ifndef CallbackCommand_hpp
#define CallbackCommand_hpp

#include <stdio.h>

#include "renderer/RenderCommand.h"

APRIL_BEGIN

class  CallbackCommand : public RenderCommand {
public:
    CallbackCommand();

    void init(float globalZOrder);
    void init(float globalZorder, const Mat4 &transform, unsigned int);

    void execute();

    std::function<void()> callback_func;
};


class  CaptureScreenCallbackCommand : public RenderCommand
{
public:
    CaptureScreenCallbackCommand();
    
    void init(float globalZOrder);
    
    std::function<void(const unsigned char*, int, int)> capture_screen_callback_func;
};

APRIL_END

#endif /* CallbackCommand_hpp */
