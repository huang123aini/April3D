//
//  EventListenerAcceleration.hpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/7.
//

#ifndef EventListenerAcceleration_hpp
#define EventListenerAcceleration_hpp

#include <stdio.h>


#include "base/CCEventListener.h"
#include "base/ccTypes.h"

/**
 * @addtogroup base
 * @{
 */

NS_CC_BEGIN

/** @class EventListenerAcceleration
 * @brief Acceleration event listener.
 * @js NA
 */
class CC_DLL EventListenerAcceleration : public EventListener
{
public:
    static const std::string LISTENER_ID;
    
    /** Create a acceleration EventListener.
     *
     * @param callback The acceleration callback method.
     * @return An autoreleased EventListenerAcceleration object.
     */
    static EventListenerAcceleration* create(const std::function<void(Acceleration*, Event*)>& callback);
    
    /** Destructor.
     */
    virtual ~EventListenerAcceleration();
    
    /// Overrides
    virtual EventListenerAcceleration* clone() override;
    virtual bool checkAvailable() override;
    
CC_CONSTRUCTOR_ACCESS:
    EventListenerAcceleration();
    
    bool init(const std::function<void(Acceleration*, Event* event)>& callback);
    
private:
    std::function<void(Acceleration*, Event*)> onAccelerationEvent;
    
    friend class LuaEventListenerAcceleration;
};

NS_CC_END

// end of base group
/// @}

#endif /* defined(__cocos2d_libs__CCAccelerometerListener__) */


#endif /* EventListenerAcceleration_hpp */
