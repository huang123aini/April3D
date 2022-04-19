//
//  Ref.hpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/2.
//

#ifndef Ref_hpp
#define Ref_hpp

#include <stdio.h>
#include "platform/AprilPlatformMacros.h"

APRIL_BEGIN

class Ref;

class Clonable {
public:
    virtual Clonable* clone() const = 0;
    
    virtual ~Clonable() {};
};


class  Ref {
public:
    void retain();

    void release();

    Ref* autorelease();

    unsigned int getReferenceCount() const;

protected:
    Ref();

public:
    virtual ~Ref();

protected:
    /**
     *引用计数
     */
    unsigned int reference_count;

    friend class AutoreleasePool;

#if APRIL_REF_LEAK_DETECTION
public:
    static void printLeaks();
#endif
};

class Node;

typedef void (Ref::*SEL_CallFunc)();
typedef void (Ref::*SEL_CallFuncN)(Node*);
typedef void (Ref::*SEL_CallFuncND)(Node*, void*);
typedef void (Ref::*SEL_CallFuncO)(Ref*);
typedef void (Ref::*SEL_MenuHandler)(Ref*);
typedef void (Ref::*SEL_SCHEDULE)(float);

#define CALLFUNC_SELECTOR(_SELECTOR) static_cast<cocos2d::SEL_CallFunc>(&_SELECTOR)
#define CALLFUNCN_SELECTOR(_SELECTOR) static_cast<cocos2d::SEL_CallFuncN>(&_SELECTOR)
#define CALLFUNCND_SELECTOR(_SELECTOR) static_cast<cocos2d::SEL_CallFuncND>(&_SELECTOR)
#define CALLFUNCO_SELECTOR(_SELECTOR) static_cast<cocos2d::SEL_CallFuncO>(&_SELECTOR)
#define MENU_SELECTOR(_SELECTOR) static_cast<cocos2d::SEL_MenuHandler>(&_SELECTOR)
#define SCHEDULE_SELECTOR(_SELECTOR) static_cast<cocos2d::SEL_SCHEDULE>(&_SELECTOR)

APRIL_END

#endif /* Ref_hpp */
