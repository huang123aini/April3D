//
//  EventListener.cpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/7.
//

#include "base/CCEventListener.h"
#include "base/CCConsole.h"

NS_CC_BEGIN

EventListener::EventListener()
{}
    
EventListener::~EventListener()
{
    CCLOGINFO("In the destructor of EventListener. %p", this);
}

bool EventListener::init(Type t, const ListenerID& listenerID, const std::function<void(Event*)>& callback)
{
    _onEvent = callback;
    _type = t;
    _listenerID = listenerID;
    _isRegistered = false;
    _paused = false;
    _isEnabled = true;
    
    return true;
}

bool EventListener::checkAvailable()
{
    return (_onEvent != nullptr);
}

NS_CC_END

