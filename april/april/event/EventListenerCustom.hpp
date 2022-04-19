//
//  EventListenerCustom.hpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/7.
//

#ifndef EventListenerCustom_hpp
#define EventListenerCustom_hpp

#include <stdio.h>
#include "base/CCEventListener.h"

/**
 * @addtogroup base
 * @{
 */

NS_CC_BEGIN

class EventCustom;

/** @class EventListenerCustom
 * @brief Custom event listener.
 * @code Usage:
 *        auto dispatcher = Director::getInstance()->getEventDispatcher();
 *     Adds a listener:
 *
 *        auto callback = [](EventCustom* event){ do_some_thing(); };
 *        auto listener = EventListenerCustom::create(callback);
 *        dispatcher->addEventListenerWithSceneGraphPriority(listener, one_node);
 *
 *     Dispatches a custom event:
 *
 *        EventCustom event("your_event_type");
 *        dispatcher->dispatchEvent(&event);
 *
 *     Removes a listener
 *
 *        dispatcher->removeEventListener(listener);
 * \endcode
 * @js cc._EventListenerCustom
 */
class CC_DLL EventListenerCustom : public EventListener
{
public:
    /** Creates an event listener with type and callback.
     * @param eventName The type of the event.
     * @param callback The callback function when the specified event was emitted.
     * @return An autoreleased EventListenerCustom object.
     */
    static EventListenerCustom* create(const std::string& eventName, const std::function<void(EventCustom*)>& callback);
    
    /// Overrides
    virtual bool checkAvailable() override;
    virtual EventListenerCustom* clone() override;
    
CC_CONSTRUCTOR_ACCESS:
    /** Constructor */
    EventListenerCustom();
    
    /** Initializes event with type and callback function */
    bool init(const ListenerID& listenerId, const std::function<void(EventCustom*)>& callback);
    
protected:
    std::function<void(EventCustom*)> _onCustomEvent;
    
    friend class LuaEventListenerCustom;
};

NS_CC_END

// end of base group
/// @}

#endif /* defined(__cocos2d_libs__CCCustomEventListener__) */


#endif /* EventListenerCustom_hpp */
