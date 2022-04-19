//
//  Event.hpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/7.
//

#ifndef Event_hpp
#define Event_hpp

#include <stdio.h>

#include "base/Ref.h"
#include "platform/AprilPlatformMacros.h"

APRIL_BEGIN

class Node;

class Event : public Ref {
public:
    enum class Type
    {
        TOUCH,
        KEYBOARD,
        ACCELERATION,
        MOUSE,
        FOCUS,
        GAME_CONTROLLER,
        CUSTOM
    };
    
    Event(Type type);
public:
   
    virtual ~Event();

    Type getType() const { return type_; }
   
    void stopPropagation() { is_stopped = true; }
    
    bool isStopped() const { return is_stopped; }
    
    Node* getCurrentTarget() { return current_target; }
    
protected:
    void setCurrentTarget(Node* target) { current_target = target; }
    
    Type type_;
    
    bool is_stopped;

    Node* current_target;
    
    friend class EventDispatcher;
};

APRIL_END

#endif /* Event_hpp */
