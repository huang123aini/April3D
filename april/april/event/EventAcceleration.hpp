//
//  EventAcceleration.hpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/7.
//

#ifndef EventAcceleration_hpp
#define EventAcceleration_hpp

#include <stdio.h>

#include "base/Event.h"
#include "base/aprilTypes.h"

APRIL_BEGIN

class EventAcceleration : public Event {
public:
    EventAcceleration(const Acceleration& acc);
    
private:
    Acceleration acc_;
    friend class EventListenerAcceleration;
};

APRIL_END

#endif /* EventAcceleration_hpp */
