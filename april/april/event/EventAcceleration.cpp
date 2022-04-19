//
//  EventAcceleration.cpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/7.
//

#include "base/EventAcceleration.h"

APRIL_BEGIN

EventAcceleration::EventAcceleration(const Acceleration& acc)
: Event(Type::ACCELERATION)
, acc_(acc) {}

APRIL_END
