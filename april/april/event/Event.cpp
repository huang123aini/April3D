//
//  Event.cpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/7.
//

#include "base/Event.h"

APRIL_BEGIN

Event::Event(Type type)
: type_(type)
, is_stopped(false)
, current_target(nullptr) {}

Event::~Event() {}

APRIL_END

