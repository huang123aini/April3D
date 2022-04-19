//
//  Vec4.hpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/6.
//

#ifndef Vec4_hpp
#define Vec4_hpp

#include <stdio.h>

/**
 *Original file from GamePlay3D: http://gameplay3d.org
 */

#ifdef __SSE__
#include <xmmintrin.h>
#endif

#include "math/MathBase.h"

MATH_BEGIN

class Mat4;

class  Vec4
{
public:
#ifdef __SSE__
    union {
        struct {
            float x;
            float y;
            float z;
            float w;
        };
        __m128 v;
    };
#else

    float x;

    float y;

    float z;

    float w;
#endif

    Vec4();

    Vec4(float xx, float yy, float zz, float ww);

    Vec4(const float* array);

    Vec4(const Vec4& p1, const Vec4& p2);

    Vec4(const Vec4& copy);

    static Vec4 fromColor(unsigned int color);

    ~Vec4();

    bool isZero() const;

    bool isOne() const;

    static float angle(const Vec4& v1, const Vec4& v2);

    void add(const Vec4& v);

    static void add(const Vec4& v1, const Vec4& v2, Vec4* dst);

    void clamp(const Vec4& min, const Vec4& max);

    static void clamp(const Vec4& v, const Vec4& min, const Vec4& max, Vec4* dst);

    float distance(const Vec4& v) const;

    float distanceSquared(const Vec4& v) const;

    float dot(const Vec4& v) const;

    static float dot(const Vec4& v1, const Vec4& v2);

    float length() const;

    float lengthSquared() const;

    void negate();

    void normalize();

    Vec4 getNormalized() const;

    void scale(float scalar);

    void set(float xx, float yy, float zz, float ww);

    void set(const float* array);

    void set(const Vec4& v);

    void set(const Vec4& p1, const Vec4& p2);

    void subtract(const Vec4& v);

    static void subtract(const Vec4& v1, const Vec4& v2, Vec4* dst);

    inline Vec4 operator+(const Vec4& v) const;

    inline Vec4& operator+=(const Vec4& v);

    inline Vec4 operator-(const Vec4& v) const;

    inline Vec4& operator-=(const Vec4& v);

    inline Vec4 operator-() const;

    inline Vec4 operator*(float s) const;

    inline Vec4& operator*=(float s);
    
    inline Vec4 operator/(float s) const;

    inline bool operator<(const Vec4& v) const;

    inline bool operator==(const Vec4& v) const;

    inline bool operator!=(const Vec4& v) const;
    
    static const Vec4 ZERO;
    
    static const Vec4 ONE;
    
    static const Vec4 UNIT_X;

    static const Vec4 UNIT_Y;

    static const Vec4 UNIT_Z;

    static const Vec4 UNIT_W;
};


inline Vec4 operator*(float x, const Vec4& v);

MATH_END

#include "math/Vec4.inl"

#endif /* Vec4_hpp */
