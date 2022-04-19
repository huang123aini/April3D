//
//  EventDispatcher.hpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/7.
//

#ifndef EventDispatcher_hpp
#define EventDispatcher_hpp

#include <stdio.h>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>
#include <set>

#include "platform/CCPlatformMacros.h"
#include "base/CCEventListener.h"
#include "base/CCEvent.h"
#include "platform/CCStdC.h"

/**
 * @addtogroup base
 * @{
 */

NS_CC_BEGIN

class Event;
class EventTouch;
class Node;
class EventCustom;
class EventListenerCustom;

class EventDispatcher : public Ref {
public:
   
    void addEventListenerWithSceneGraphPriority(EventListener* listener, Node* node);


    void addEventListenerWithFixedPriority(EventListener* listener, int fixedPriority);

    EventListenerCustom* addCustomEventListener(const std::string &eventName, const std::function<void(EventCustom*)>& callback);

    void removeEventListener(EventListener* listener);

    void removeEventListenersForType(EventListener::Type listenerType);

    void removeEventListenersForTarget(Node* target, bool recursive = false);
    
    void removeCustomEventListeners(const std::string& customEventName);

    void removeAllEventListeners();

    void pauseEventListenersForTarget(Node* target, bool recursive = false);
    
    void resumeEventListenersForTarget(Node* target, bool recursive = false);
    
    void setPriority(EventListener* listener, int fixedPriority);

    void setEnabled(bool isEnabled);

    bool isEnabled() const;

    void dispatchEvent(Event* event);

    void dispatchCustomEvent(const std::string &eventName, void *optionalUserData = nullptr);

    bool hasEventListener(const EventListener::ListenerID& listenerID) const;

    EventDispatcher();

    ~EventDispatcher();

#if NODE_DEBUG_VERIFY_EVENT_LISTENERS && APRIL3D_DEBUG > 0
    
    void debugCheckNodeHasNoEventListenersOnDestruction(Node* node);
    
#endif

protected:
    friend class Node;
    
    void setDirtyForNode(Node* node);
  
    class EventListenerVector
    {
    public:
        EventListenerVector();
        ~EventListenerVector();
        size_t size() const;
        bool empty() const;
        
        void push_back(EventListener* item);
        void clearSceneGraphListeners();
        void clearFixedListeners();
        void clear();
        
        std::vector<EventListener*>* getFixedPriorityListeners() const { return _fixedListeners; }
        std::vector<EventListener*>* getSceneGraphPriorityListeners() const { return _sceneGraphListeners; }
        ssize_t getGt0Index() const { return _gt0Index; }
        void setGt0Index(ssize_t index) { _gt0Index = index; }
    private:
        std::vector<EventListener*>* _fixedListeners;
        std::vector<EventListener*>* _sceneGraphListeners;
        ssize_t _gt0Index;
    };
    
    void addEventListener(EventListener* listener);
    
    void forceAddEventListener(EventListener* listener);
    
    EventListenerVector* getListeners(const EventListener::ListenerID& listenerID) const;
    
    void updateDirtyFlagForSceneGraph();
    
    void removeEventListenersForListenerID(const EventListener::ListenerID& listenerID);
    
    void sortEventListeners(const EventListener::ListenerID& listenerID);
    
    void sortEventListenersOfSceneGraphPriority(const EventListener::ListenerID& listenerID, Node* rootNode);
    

    void sortEventListenersOfFixedPriority(const EventListener::ListenerID& listenerID);
    
    void updateListeners(Event* event);

    void dispatchTouchEvent(EventTouch* event);
    
    void associateNodeAndEventListener(Node* node, EventListener* listener);
    
    void dissociateNodeAndEventListener(Node* node, EventListener* listener);
    
    void dispatchEventToListeners(EventListenerVector* listeners, const std::function<bool(EventListener*)>& onEvent);
    
    void dispatchTouchEventToListeners(EventListenerVector* listeners, const std::function<bool(EventListener*)>& onEvent);
    
    void releaseListener(EventListener* listener);
    
    enum class DirtyFlag {
        NONE = 0,
        FIXED_PRIORITY = 1 << 0,
        SCENE_GRAPH_PRIORITY = 1 << 1,
        ALL = FIXED_PRIORITY | SCENE_GRAPH_PRIORITY
    };
    
    void setDirty(const EventListener::ListenerID& listenerID, DirtyFlag flag);
    
    void visitTarget(Node* node, bool isRootNode);

    void cleanToRemovedListeners();

    std::unordered_map<EventListener::ListenerID, EventListenerVector*> listener_map;
    
    std::unordered_map<EventListener::ListenerID, DirtyFlag> priority_dirty_flag_map;
    
    std::unordered_map<Node*, std::vector<EventListener*>*> node_listeners_map;
    
    std::unordered_map<Node*, int> node_priority_map;
    
    std::unordered_map<float, std::vector<Node*>> global_z_order_node_map;
    
    std::vector<EventListener*> to_added_listeners;

    std::vector<EventListener*> to_removed_listeners;

    std::set<Node*> dirty_nodes;
    
    int in_dispatch;
    
    bool is_enabled;
    
    int node_priority_index;
    
    std::set<std::string> internal_custom_listener_IDs;
};


APRIL_END

#endif /* EventDispatcher_hpp */
