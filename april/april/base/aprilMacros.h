//
//  aprilMacros.h
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/3.
//

#ifndef aprilMacros_h
#define aprilMacros_h

#include <iostream>

#define APRIL_DEBUG 1


#define CHECK_GL_ERROR_DEBUG() \
    do { \
        GLenum __error = glGetError(); \
        if(__error) { \
            printf("OpenGL error 0x%04X in %s %s %d\n", __error, __FILE__, __FUNCTION__, __LINE__); \
        } \
    } while (false)
#define CHECK_GL_ERROR_ABORT() \
    do { \
        GLenum __error = glGetError(); \
        if(__error) { \
            printf("OpenGL error 0x%04X in %s %s %d\n", __error, __FILE__, __FUNCTION__, __LINE__); \
            assert(false);\
        } \
    } while (false)



#ifndef FLT_EPSILON
#define FLT_EPSILON     1.192092896e-07F
#endif


/**
 *角度转换为弧度
 */
#define DEGREES_TO_RADIANS(__ANGLE__) ((__ANGLE__) * 0.01745329252f) // PI / 180

/**
 *弧度转换为角度
 */
#define RADIANS_TO_DEGREES(__ANGLE__) ((__ANGLE__) * 57.29577951f) // PI * 180

#endif /* aprilMacros_h */
