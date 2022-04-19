//
//  Vertex.hpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/6.
//

#ifndef Vertex_hpp
#define Vertex_hpp

#include <stdio.h>
#include "base/aprilTypes.h"

MATH_BEGIN

/** @file CCVertex.h */

/** converts a line to a polygon */
void  aprilVertexLineToPolygon(Vec2 *points, float stroke, Vec2 *vertices, unsigned int offset, unsigned int nuPoints);

/** returns whether or not the line intersects */
bool  aprilVertexLineIntersect(float Ax, float Ay,
                             float Bx, float By,
                             float Cx, float Cy,
                             float Dx, float Dy, float *T);

MATH_END

#endif /* Vertex_hpp */
