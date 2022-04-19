//
//  Vec3.hpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/6.
//

#ifndef Vec3_hpp
#define Vec3_hpp

#include <stdio.h>
#include <cmath>
#include "math/MathBase.h"

MATH_BEGIN

class Mat4;
class Quaternion;

class Vec3 {
public:
    float x;
    float y;
    float z;
    Vec3();

    Vec3(float xx, float yy, float zz);

    Vec3(const float* array);

    Vec3(const Vec3& p1, const Vec3& p2);

    static Vec3 fromColor(unsigned int color);

    inline bool isZero() const;

    inline bool isOne() const;

    static float angle(const Vec3& v1, const Vec3& v2);

    inline void add(const Vec3& v);

    inline void add(float xx, float yy, float zz);

    static void add(const Vec3& v1, const Vec3& v2, Vec3* dst);

    void clamp(const Vec3& min, const Vec3& max);
    
    static void clamp(const Vec3& v, const Vec3& min, const Vec3& max, Vec3* dst);

    void cross(const Vec3& v);

    static void cross(const Vec3& v1, const Vec3& v2, Vec3* dst);

    float distance(const Vec3& v) const;

    float distanceSquared(const Vec3& v) const;

    float dot(const Vec3& v) const;

    static float dot(const Vec3& v1, const Vec3& v2);

    inline float length() const;

    inline float lengthSquared() const;

    /**
     * Negates this vector.
     */
    inline void negate();

    void normalize();

    Vec3 getNormalized() const;

    inline void scale(float scalar);

    inline void set(float xx, float yy, float zz);

    inline void set(const float* array);

    inline void set(const Vec3& v);

    inline void set(const Vec3& p1, const Vec3& p2);

    inline void setZero();

    inline void subtract(const Vec3& v);

    static void subtract(const Vec3& v1, const Vec3& v2, Vec3* dst);

    void smooth(const Vec3& target, float elapsedTime, float responseTime);

    inline Vec3 lerp(const Vec3& target, float alpha) const;

    inline Vec3 operator+(const Vec3& v) const;

    inline Vec3& operator+=(const Vec3& v);

    inline Vec3 operator-(const Vec3& v) const;

    inline Vec3& operator-=(const Vec3& v);

    inline Vec3 operator-() const;

    inline Vec3 operator*(float s) const;

    inline Vec3& operator*=(float s);
    
    inline Vec3 operator/(float s) const;

    inline bool operator < (const Vec3& rhs) const {
        if (x < rhs.x && y < rhs.y && z < rhs.z)
            return true;
        return false;
    }

    inline bool operator >(const Vec3& rhs) const {
        if (x > rhs.x && y > rhs.y && z > rhs.z)
            return true;
        return false;
    }

    inline bool operator==(const Vec3& v) const;

    inline bool operator!=(const Vec3& v) const;
    
    static const Vec3 ZERO;
    
    static const Vec3 ONE;
    
    static const Vec3 UNIT_X;
  
    static const Vec3 UNIT_Y;
  
    static const Vec3 UNIT_Z;
};

inline Vec3 operator*(float x, const Vec3& v);

MATH_END

#include "math/Vec3.inl"

#endif /* Vec3_hpp */
