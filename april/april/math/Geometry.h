//
//  Geometry.hpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/6.
//

#ifndef Geometry_hpp
#define Geometry_hpp

#include <stdio.h>
#include "platform/AprilPlatformMacros.h"
#include "base/aprilMacros.h"
#include "math/Math.h"

APRIL_BEGIN

class Size
{
public:
    float width;
    float height;
public:
    /**Conversion from Vec2 to Size.*/
    operator Vec2() const {
        return Vec2(width, height);
    }

public:
    Size();
    
    Size(float width, float height);
    
    Size(const Size& other);
    
    explicit Size(const Vec2& point);
    
    Size& operator= (const Size& other);
  
    Size& operator= (const Vec2& point);
    
    Size operator+(const Size& right) const;
   
    Size operator-(const Size& right) const;
   
    Size operator*(float a) const;
   
    Size operator/(float a) const;
   
    void setSize(float width, float height);
  
    bool equals(const Size& target) const;

    static const Size ZERO;
};

class  Rect {
public:
    Vec2 origin;
    
    Size  size;

public:
    Rect();
  
    Rect(float x, float y, float width, float height);
   
    Rect(const Vec2& pos, const Size& dimension);
  
    Rect(const Rect& other);
  
    Rect& operator= (const Rect& other);
   
    void setRect(float x, float y, float width, float height);
   
    float getMinX() const;
 
    float getMidX() const;
   
    float getMaxX() const;
  
    float getMinY() const;
  
    float getMidY() const;
   
    float getMaxY() const;
   
    bool equals(const Rect& rect) const;
   
    bool containsPoint(const Vec2& point) const;
   
    bool intersectsRect(const Rect& rect) const;
   
    bool intersectsCircle(const Vec2& center, float radius) const;
   
    Rect unionWithRect(const Rect & rect) const;

    void merge(const Rect& rect);

    static const Rect ZERO;
};

APRIL_END

#endif /* Geometry_hpp */
