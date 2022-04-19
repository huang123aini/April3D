//
//  TransformUtils.hpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/6.
//

#ifndef TransformUtils_hpp
#define TransformUtils_hpp

#include <stdio.h>
#include "math/MathBase.h"

MATH_BEGIN

struct AffineTransform;
/**
 *Conversion between mat4*4 and AffineTransform.
 */
 void CGAffineToGL(const AffineTransform &t, float *m);
 void GLToCGAffine(const float *m, AffineTransform *t);

MATH_END

#endif /* TransformUtils_hpp */
