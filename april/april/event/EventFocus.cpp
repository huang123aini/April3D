//
//  EventFocus.cpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/7.
//

#include "base/CCEventFocus.h"

NS_CC_BEGIN


EventFocus::EventFocus(ui::Widget *widgetLoseFocus, ui::Widget* widgetGetFocus)
:Event(Type::FOCUS),
_widgetGetFocus(widgetGetFocus),
_widgetLoseFocus(widgetLoseFocus)
{
    
}




NS_CC_END
