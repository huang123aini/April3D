//
//  EventListenerMouse.hpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/7.
//

#ifndef EventListenerMouse_hpp
#define EventListenerMouse_hpp

#include <stdio.h>

#include "base/CCEventListener.h"
#include "base/CCEventMouse.h"

/**
 * @addtogroup base
 * @{
 */

NS_CC_BEGIN

class Event;

/** @class EventListenerMouse
 * @brief Mouse event listener.
 * @js cc._EventListenerMouse
 */
class CC_DLL EventListenerMouse : public EventListener
{
public:
    static const std::string LISTENER_ID;
    
    /** Create a mouse event listener.
     *
     * @return An autoreleased EventListenerMouse object.
     */
    static EventListenerMouse* create();

    /// Overrides
    virtual EventListenerMouse* clone() override;
    virtual bool checkAvailable() override;

    std::function<void(EventMouse* event)> onMouseDown;
    std::function<void(EventMouse* event)> onMouseUp;
    std::function<void(EventMouse* event)> onMouseMove;
    std::function<void(EventMouse* event)> onMouseScroll;

CC_CONSTRUCTOR_ACCESS:
    EventListenerMouse();
    bool init();
};

NS_CC_END

// end of base group
/// @}

#endif /* defined(__cocos2d_libs__CCMouseEventListener__) */


#endif /* EventListenerMouse_hpp */
