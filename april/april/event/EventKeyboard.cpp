//
//  EventKeyboard.cpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/7.
//

#include "base/CCEventKeyboard.h"

NS_CC_BEGIN

EventKeyboard::EventKeyboard(KeyCode keyCode, bool isPressed)
: Event(Type::KEYBOARD)
, _keyCode(keyCode)
, _isPressed(isPressed)
{}

NS_CC_END
