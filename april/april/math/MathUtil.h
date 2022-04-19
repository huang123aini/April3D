//
//  MathUtil.hpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/6.
//

#ifndef MathUtil_hpp
#define MathUtil_hpp

#include <stdio.h>

#ifdef __SSE__
#include <xmmintrin.h>
#endif

#include "math/MathBase.h"

MATH_BEGIN

class  MathUtil {
    friend class Mat4;
    friend class Vec3;

public:

    static void smooth(float* x, float target, float elapsedTime, float responseTime);

    static void smooth(float* x, float target, float elapsedTime, float riseTime, float fallTime);
    
    static float lerp(float from, float to, float alpha);
private:
    //Indicates that if neon is enabled
    static bool isNeon32Enabled();
    static bool isNeon64Enabled();
private:
#ifdef __SSE__
    static void addMatrix(const __m128 m[4], float scalar, __m128 dst[4]);
    
    static void addMatrix(const __m128 m1[4], const __m128 m2[4], __m128 dst[4]);
    
    static void subtractMatrix(const __m128 m1[4], const __m128 m2[4], __m128 dst[4]);
    
    static void multiplyMatrix(const __m128 m[4], float scalar, __m128 dst[4]);
    
    static void multiplyMatrix(const __m128 m1[4], const __m128 m2[4], __m128 dst[4]);
    
    static void negateMatrix(const __m128 m[4], __m128 dst[4]);
    
    static void transposeMatrix(const __m128 m[4], __m128 dst[4]);
        
    static void transformVec4(const __m128 m[4], const __m128& v, __m128& dst);
#endif
    static void addMatrix(const float* m, float scalar, float* dst);

    static void addMatrix(const float* m1, const float* m2, float* dst);

    static void subtractMatrix(const float* m1, const float* m2, float* dst);

    static void multiplyMatrix(const float* m, float scalar, float* dst);

    static void multiplyMatrix(const float* m1, const float* m2, float* dst);

    static void negateMatrix(const float* m, float* dst);

    static void transposeMatrix(const float* m, float* dst);

    static void transformVec4(const float* m, float x, float y, float z, float w, float* dst);

    static void transformVec4(const float* m, const float* v, float* dst);

    static void crossVec3(const float* v1, const float* v2, float* dst);

};

MATH_END

#define MATRIX_SIZE ( sizeof(float) * 16)

#endif /* MathUtil_hpp */
