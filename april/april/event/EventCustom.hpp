//
//  EventCustom.hpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/7.
//

#ifndef EventCustom_hpp
#define EventCustom_hpp

#include <stdio.h>
#include <string>
#include "base/Event.h"

APRIL_BEGIN

class EventCustom : public Event {
public:
  
    EventCustom(const std::string& eventName);
    
    void setUserData(void* data) { user_data = data; }
    
    void* getUserData() const { return user_data; }
    
    const std::string& getEventName() const { return event_name; }
protected:
    void* user_data;       ///< User data
    std::string event_name;
};

APRIL_END

#endif /* EventCustom_hpp */
