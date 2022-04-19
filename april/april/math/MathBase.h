//
//  MathBase.h
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/6.
//

#ifndef MathBase_h
#define MathBase_h

#include <memory>
#include <string.h>
#include "platform/AprilPlatformMacros.h"

#define MATH_BEGIN                     namespace april3d {
#define MATH_END                       }


/**
 *角度转弧度
 */
#define MATH_DEG_TO_RAD(x)          ((x) * 0.0174532925f)

/**
 *弧度转角度
 */
#define MATH_RAD_TO_DEG(x)          ((x)* 57.29577951f)


#define MATH_FLOAT_SMALL            1.0e-37f

#define MATH_TOLERANCE              2e-37f

#define MATH_PIOVER2                1.57079632679489661923f

#define MATH_EPSILON                0.000001f

#endif /* MathBase_h */
