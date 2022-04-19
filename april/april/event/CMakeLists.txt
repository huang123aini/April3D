cmake_minimum_required(VERSION 3.10)

#添加场景模块文件

set(APRIL_EVENT_HEADER
    event/Event.h
    event/EventAcceleration.h
    event/EventController.h
    event/EventCustom.h
    event/EventDispatcher.h
    event/Dispatcher.h
    event/EventFocus.h
    event/EventKeyboard.h
    event/EventListener.h
    event/EventListenerAcceleration.h
    event/EventListenerController.h
    event/EventListenerCustom.h
    event/EventListenerFocus.h
    event/EventListenerKeyboard.h
    event/EventListenerMouse.h
    event/EventListenerTouch.h
    event/EventMouse.h
    event/EventTouch.h
    event/EventType.h
    )
   

set(APRIL_EVENT_SRC
    event/Event.cpp
    event/EventAcceleration.cpp
    event/EventController.cpp
    event/EventCustom.cpp
    event/EventDispatcher.cpp
    event/Dispatcher.cpp
    event/EventFocus.cpp
    event/EventKeyboard.cpp
    event/EventListener.cpp
    event/EventListenerAcceleration.cpp
    event/EventListenerController.cpp
    event/EventListenerCustom.cpp
    event/EventListenerFocus.cpp
    event/EventListenerKeyboard.cpp
    event/EventListenerMouse.cpp
    event/EventListenerTouch.cpp
    event/EventMouse.cpp
    event/EventTouch.cpp
    )
