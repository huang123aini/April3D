//
//  AffineTransform.hpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/6.
//

#ifndef AffineTransform_hpp
#define AffineTransform_hpp

#include <stdio.h>
#include "platform/AprilPlatformMacros.h"

#include "math/Geometry.h"
#include "math/Math.h"

APRIL_BEGIN

/**
 Affine transform
 a   b    0
 c   d    0
 tx  ty   1
 
 Identity
 1   0    0
 0   1    0
 0   0    1
 */
struct  AffineTransform {
    float a, b, c, d;
    float tx, ty;

    static const AffineTransform IDENTITY;
};


AffineTransform AffineTransformMake__(float a, float b, float c, float d, float tx, float ty);

#define AffineTransformMake AffineTransformMake__

/**Multiply point (x,y,1) by a  affine transform.*/

Vec2 PointApplyAffineTransform__(const Vec2& point, const AffineTransform& t);

#define PointApplyAffineTransform PointApplyAffineTransform__

/**Multiply size (width,height,0) by a  affine transform.*/
Size __CCSizeApplyAffineTransform(const Size& size, const AffineTransform& t);

#define SizeApplyAffineTransform SizeApplyAffineTransform__

/**Make identity affine transform.*/
AffineTransform AffineTransformMakeIdentity();

/**Transform Rect, which will transform the four vertices of the point.*/
Rect RectApplyAffineTransform(const Rect& rect, const AffineTransform& anAffineTransform);
/**@{
 Transform vec2 and Rect by Mat4.
 */
Rect RectApplyTransform(const Rect& rect, const Mat4& transform);
Vec2 PointApplyTransform(const Vec2& point, const Mat4& transform);
/**@}*/
/**
 Translation, equals
 1  0  1
 0  1  0   * affine transform
 tx ty 1
 */
 AffineTransform AffineTransformTranslate(const AffineTransform& t, float tx, float ty);
/**
 Rotation, equals
 cos(angle)   sin(angle)   0
 -sin(angle)  cos(angle)   0  * AffineTransform
 0            0            1
 */
 AffineTransform AffineTransformRotate(const AffineTransform& aTransform, float anAngle);
/**
 Scale, equals
 sx   0   0
 0    sy  0  * affineTransform
 0    0   1
 */
 AffineTransform AffineTransformScale(const AffineTransform& t, float sx, float sy);
/**Concat two affine transform, t1 * t2*/
 AffineTransform AffineTransformConcat(const AffineTransform& t1, const AffineTransform& t2);
/**Compare affine transform.*/
 bool AffineTransformEqualToTransform(const AffineTransform& t1, const AffineTransform& t2);
/**Get the inverse of affine transform.*/
 AffineTransform AffineTransformInvert(const AffineTransform& t);
/**Concat Mat4, return t1 * t2.*/
 Mat4 TransformConcat(const Mat4& t1, const Mat4& t2);

extern const AffineTransform AffineTransformIdentity;

APRIL_END

#endif /* AffineTransform_hpp */
