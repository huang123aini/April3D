//
//  EventCustom.cpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/7.
//

#include "base/CCEventCustom.h"
#include "base/CCEvent.h"

NS_CC_BEGIN

EventCustom::EventCustom(const std::string& eventName)
: Event(Type::CUSTOM)
, _userData(nullptr)
, _eventName(eventName)
{
}

NS_CC_END
