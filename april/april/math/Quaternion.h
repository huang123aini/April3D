//
//  Quaternion.hpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/6.
//

#ifndef Quaternion_hpp
#define Quaternion_hpp

#include <stdio.h>
#include "math/Vec3.h"
#include "math/Mat4.h"

MATH_BEGIN

class Mat4;

/**
 * Defines a 4-element quaternion that represents the orientation of an object in space.
 *
 * Quaternions are typically used as a replacement for euler angles and rotation matrices as a way to achieve smooth interpolation and avoid gimbal lock.
 *
 * Note that this quaternion class does not automatically keep the quaternion normalized. Therefore, care must be taken to normalize the quaternion when necessary, by calling the normalize method.
 * This class provides three methods for doing quaternion interpolation: lerp, slerp, and squad.
 *
 * lerp (linear interpolation): the interpolation curve gives a straight line in quaternion space. It is simple and fast to compute. The only problem is that it does not provide constant angular velocity. Note that a constant velocity is not necessarily a requirement for a curve;
 * slerp (spherical linear interpolation): the interpolation curve forms a great arc on the quaternion unit sphere. Slerp provides constant angular velocity;
 * squad (spherical spline interpolation): interpolating between a series of rotations using slerp leads to the following problems:
 * - the curve is not smooth at the control points;
 * - the angular velocity is not constant;
 * - the angular velocity is not continuous at the control points.
 *
 * Since squad is continuously differentiable, it remedies the first and third problems mentioned above.
 * The slerp method provided here is intended for interpolation of principal rotations. It treats +q and -q as the same principal rotation and is at liberty to use the negative of either input. The resulting path is always the shorter arc.
 *
 * The lerp method provided here interpolates strictly in quaternion space. Note that the resulting path may pass through the origin if interpolating between a quaternion and its exact negative.
 *
 * As an example, consider the following quaternions:
 *
 * q1 = (0.6, 0.8, 0.0, 0.0),
 * q2 = (0.0, 0.6, 0.8, 0.0),
 * q3 = (0.6, 0.0, 0.8, 0.0), and
 * q4 = (-0.8, 0.0, -0.6, 0.0).
 * For the point p = (1.0, 1.0, 1.0), the following figures show the trajectories of p using lerp, slerp, and squad.
 */
class Quaternion {
    friend class Curve;
    friend class Transform;

public:

    float x;
    
    float y;

    float z;

    float w;
    
    Quaternion();

    Quaternion(float xx, float yy, float zz, float ww);

    Quaternion(float* array);

    Quaternion(const Mat4& m);

    Quaternion(const Vec3& axis, float angle);

    static const Quaternion& identity();

    static const Quaternion& zero();

    bool isIdentity() const;

    bool isZero() const;

    static void createFromRotationMatrix(const Mat4& m, Quaternion* dst);

    static void createFromAxisAngle(const Vec3& axis, float angle, Quaternion* dst);

    void conjugate();

    Quaternion getConjugated() const;

    bool inverse();

    Quaternion getInversed() const;

    void multiply(const Quaternion& q);

    static void multiply(const Quaternion& q1, const Quaternion& q2, Quaternion* dst);

    void normalize();

    Quaternion getNormalized() const;

    void set(float xx, float yy, float zz, float ww);

    void set(float* array);

    void set(const Mat4& m);

    void set(const Vec3& axis, float angle);

    void set(const Quaternion& q);

    void setIdentity();

    float toAxisAngle(Vec3* e) const;

    static void lerp(const Quaternion& q1, const Quaternion& q2, float t, Quaternion* dst);
    
    static void slerp(const Quaternion& q1, const Quaternion& q2, float t, Quaternion* dst);
    
    static void squad(const Quaternion& q1, const Quaternion& q2, const Quaternion& s1, const Quaternion& s2, float t, Quaternion* dst);

    inline Quaternion operator*(const Quaternion& q) const;

    inline Vec3 operator*(const Vec3& v) const;

    inline Quaternion& operator*=(const Quaternion& q);
 
    static const Quaternion ZERO;

private:

    static void slerp(float q1x, float q1y, float q1z, float q1w, float q2x, float q2y, float q2z, float q2w, float t, float* dstx, float* dsty, float* dstz, float* dstw);

    static void slerpForSquad(const Quaternion& q1, const Quaternion& q2, float t, Quaternion* dst);
};

MATH_END

#include "math/Quaternion.inl"

#endif /* Quaternion_hpp */
