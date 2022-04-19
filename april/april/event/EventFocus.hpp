//
//  EventFocus.hpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/7.
//

#ifndef EventFocus_hpp
#define EventFocus_hpp


#include "base/CCEvent.h"

/**
 * @addtogroup base
 * @{
 */

NS_CC_BEGIN

namespace ui {
    class Widget;
}

/** @class EventFocus
 * @brief Focus event.
 */
class CC_DLL EventFocus : public Event
{
public:
    /** Constructor.
     *
     * @param widgetLoseFocus The widget which lose focus.
     * @param widgetGetFocus The widget which get focus.
     * @js ctor
     */
    EventFocus(ui::Widget* widgetLoseFocus, ui::Widget* widgetGetFocus);
    
private:
    ui::Widget *_widgetGetFocus;
    ui::Widget *_widgetLoseFocus;
    
    friend class EventListenerFocus;
};


NS_CC_END

// end of base group
/// @}

#endif /* defined(__cocos2d_libs__CCEventFocus__) */


#endif /* EventFocus_hpp */
