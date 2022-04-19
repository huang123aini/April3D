//
//  EventController.cpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/7.
//

#include "base/CCEventController.h"

APRIL_BEGIN

EventController::EventController(ControllerEventType type, Controller* controller, int keyCode)
: Event(Type::GAME_CONTROLLER)
, controller_event_type(type)
, controller_(controller)
, key_code(keyCode)
, is_connected(true){}

EventController::EventController(ControllerEventType type, Controller* controller, bool isConnected)
: Event(Type::GAME_CONTROLLER)
, controller_event_type(type)
, controller_(controller)
, key_code(0)
, is_connected(isConnected) {}

APRIL_END
