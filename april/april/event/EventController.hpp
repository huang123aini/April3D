//
//  EventController.hpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/7.
//

#ifndef EventController_hpp
#define EventController_hpp

#include <stdio.h>

#include "platform/AprilPlatformMacros.h"
#include "base/Event.h"

APRIL_BEGIN

class Controller;
class EventListenerController;

class EventController : public Event {
public:
    enum class ControllerEventType {
        CONNECTION,
        BUTTON_STATUS_CHANGED,
        AXIS_STATUS_CHANGED,
    };
    
    EventController(ControllerEventType type, Controller* controller, int keyCode);

    EventController(ControllerEventType type, Controller* controller, bool isConnected);

    ControllerEventType getControllerEventType() const { return controller_event_type; }
    
    Controller* getController() const { return controller_; }

    int getKeyCode() const{ return key_code; }
    void setKeyCode(int keyCode) { key_code = keyCode;}

    void setConnectStatus(bool isConnected) {is_connected = isConnected;}

    bool isConnected() const { return is_connected; }
    
protected:
    ControllerEventType controller_event_type;
    Controller* controller_;
    int key_code;
    bool is_connected;

    friend class EventListenerController;
};

APRIL_END

#endif /* EventController_hpp */
