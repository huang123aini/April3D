//
//  EventListenerFocus.cpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/7.
//

#include "base/CCEventListenerFocus.h"
#include "base/CCEventFocus.h"
#include "base/ccMacros.h"

NS_CC_BEGIN


const std::string EventListenerFocus::LISTENER_ID = "__cc_focus_event";


EventListenerFocus::EventListenerFocus()
:onFocusChanged(nullptr)
{
    
}

EventListenerFocus::~EventListenerFocus()
{
    CCLOGINFO("In the destructor of EventListenerFocus, %p", this);
}

EventListenerFocus* EventListenerFocus::create()
{
    EventListenerFocus* ret = new (std::nothrow) EventListenerFocus;
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

EventListenerFocus* EventListenerFocus::clone()
{
    EventListenerFocus* ret = new (std::nothrow) EventListenerFocus;
    if (ret && ret->init()) {
        ret->autorelease();
        
        ret->onFocusChanged = onFocusChanged;
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool EventListenerFocus::init()
{
    auto listener = [this](Event* event){
        auto focusEvent = static_cast<EventFocus*>(event);
        onFocusChanged(focusEvent->_widgetLoseFocus, focusEvent->_widgetGetFocus);
    };
    if (EventListener::init(Type::FOCUS, LISTENER_ID, listener)) {
        return true;
    }
    return false;
}

bool EventListenerFocus::checkAvailable()
{
    if (onFocusChanged == nullptr)
    {
        CCASSERT(false, "Invalid EventListenerFocus!");
        return false;
    }
    
    return true;
}



NS_CC_END
