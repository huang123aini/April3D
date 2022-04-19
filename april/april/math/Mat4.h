//
//  Mat4.hpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/6.
//

#ifndef Mat4_hpp
#define Mat4_hpp

#include <stdio.h>

#include "base/aprilMacros.h"

#include "math/Vec3.h"
#include "math/Vec4.h"

#ifdef __SSE__
#include <xmmintrin.h>
#endif

MATH_BEGIN

/**
 * Defines a 4 x 4 floating point matrix representing a 3D transformation.
 *
 * Vectors are treated as columns, resulting in a matrix that is represented as follows,
 * where x, y and z are the translation components of the matrix:
 *
 *     1  0  0  x
 *     0  1  0  y
 *     0  0  1  z
 *     0  0  0  1
 */
class Mat4 {
public:

#ifdef __SSE__
    union {
        __m128 col[4];
        float m[16];
    };
#else
    float m[16];
#endif

    Mat4();

    Mat4(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24,
           float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44);

    /**
     * Creates a matrix initialized to the specified column-major array.
     *
     * The passed-in array is in column-major order, so the memory layout of the array is as follows:
     *
     *     0   4   8   12
     *     1   5   9   13
     *     2   6   10  14
     *     3   7   11  15
     *
     * @param mat An array containing 16 elements in column-major order.
     */
    Mat4(const float* mat);

    Mat4(const Mat4& copy);

    /**
     * Destructor.
     */
    ~Mat4();

    /**
     * Creates a view matrix based on the specified input parameters.
     * 眼睛的位置，目标位置，仰角
     */
    static void createLookAt(const Vec3& eyePosition, const Vec3& targetPosition, const Vec3& up, Mat4* dst);

    static void createLookAt(float eyePositionX, float eyePositionY, float eyePositionZ,
                             float targetCenterX, float targetCenterY, float targetCenterZ,
                             float upX, float upY, float upZ, Mat4* dst);

    /**
     * Builds a perspective projection matrix based on a field of view and returns by value.
     */
    static void createPerspective(float fieldOfView, float aspectRatio, float zNearPlane, float zFarPlane, Mat4* dst);

    static void createOrthographic(float width, float height, float zNearPlane, float zFarPlane, Mat4* dst);

    /**
     * Creates an orthographic projection matrix.
     */
    static void createOrthographicOffCenter(float left, float right, float bottom, float top,
                                            float zNearPlane, float zFarPlane, Mat4* dst);

    /**
     * Creates a spherical billboard that rotates around a specified object position.
     */
    static void createBillboard(const Vec3& objectPosition, const Vec3& cameraPosition,
                                const Vec3& cameraUpVector, Mat4* dst);

    /**
     * Creates a spherical billboard that rotates around a specified object position with
     * provision for a safe default orientation.
     */
    static void createBillboard(const Vec3& objectPosition, const Vec3& cameraPosition,
                                const Vec3& cameraUpVector, const Vec3& cameraForwardVector,
                                Mat4* dst);

    static void createScale(const Vec3& scale, Mat4* dst);

    static void createScale(float xScale, float yScale, float zScale, Mat4* dst);

    static void createRotation(const Quaternion& quat, Mat4* dst);

    static void createRotation(const Vec3& axis, float angle, Mat4* dst);

    static void createRotationX(float angle, Mat4* dst);

    static void createRotationY(float angle, Mat4* dst);

    static void createRotationZ(float angle, Mat4* dst);

    static void createTranslation(const Vec3& translation, Mat4* dst);

    static void createTranslation(float xTranslation, float yTranslation, float zTranslation, Mat4* dst);

    void add(float scalar);

    void add(float scalar, Mat4* dst);

    void add(const Mat4& mat);

    static void add(const Mat4& m1, const Mat4& m2, Mat4* dst);

    bool decompose(Vec3* scale, Quaternion* rotation, Vec3* translation) const;

    float determinant() const;

    void getScale(Vec3* scale) const;

    bool getRotation(Quaternion* rotation) const;

    void getTranslation(Vec3* translation) const;

    void getUpVector(Vec3* dst) const;

    void getDownVector(Vec3* dst) const;

    void getLeftVector(Vec3* dst) const;

    void getRightVector(Vec3* dst) const;

    void getForwardVector(Vec3* dst) const;

    void getBackVector(Vec3* dst) const;

    bool inverse();

    Mat4 getInversed() const;

    bool isIdentity() const;

    void multiply(float scalar);

    void multiply(float scalar, Mat4* dst) const;

    static void multiply(const Mat4& mat, float scalar, Mat4* dst);

    void multiply(const Mat4& mat);

    static void multiply(const Mat4& m1, const Mat4& m2, Mat4* dst);

    /**
     * Negates this matrix.
     */
    void negate();

    Mat4 getNegated() const;

    void rotate(const Quaternion& q);

    void rotate(const Quaternion& q, Mat4* dst) const;

    void rotate(const Vec3& axis, float angle);

    void rotate(const Vec3& axis, float angle, Mat4* dst) const;

    void rotateX(float angle);

    void rotateX(float angle, Mat4* dst) const;

    void rotateY(float angle);

    void rotateY(float angle, Mat4* dst) const;

    void rotateZ(float angle);

    void rotateZ(float angle, Mat4* dst) const;

    void scale(float value);

    void scale(float value, Mat4* dst) const;

    void scale(float xScale, float yScale, float zScale);

    void scale(float xScale, float yScale, float zScale, Mat4* dst) const;

    void scale(const Vec3& s);

    void scale(const Vec3& s, Mat4* dst) const;

    /**
     * Sets the values of this matrix.
     */
    void set(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24,
             float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44);

    void set(const float* mat);

    void set(const Mat4& mat);

    void setIdentity();

    void setZero();

    void subtract(const Mat4& mat);

    static void subtract(const Mat4& m1, const Mat4& m2, Mat4* dst);

    inline void transformPoint(Vec3* point) const { /**GP_ASSERT(point);*/ transformVector(point->x, point->y, point->z, 1.0f, point); }

    inline void transformPoint(const Vec3& point, Vec3* dst) const { /**GP_ASSERT(dst);*/ transformVector(point.x, point.y, point.z, 1.0f, dst); }

    void transformVector(Vec3* vector) const;

    void transformVector(const Vec3& vector, Vec3* dst) const;

    void transformVector(float x, float y, float z, float w, Vec3* dst) const;

    void transformVector(Vec4* vector) const;

    void transformVector(const Vec4& vector, Vec4* dst) const;

    void translate(float x, float y, float z);

    void translate(float x, float y, float z, Mat4* dst) const;

    void translate(const Vec3& t);

    void translate(const Vec3& t, Mat4* dst) const;

    void transpose();

    Mat4 getTransposed() const;

    inline Mat4 operator+(const Mat4& mat) const;
    
    inline Mat4& operator+=(const Mat4& mat);

    inline Mat4 operator-(const Mat4& mat) const;

    inline Mat4& operator-=(const Mat4& mat);

    inline Mat4 operator-() const;

    inline Mat4 operator*(const Mat4& mat) const;

    inline Mat4& operator*=(const Mat4& mat);

    static const Mat4 ZERO;

    static const Mat4 IDENTITY;

private:

    static void createBillboardHelper(const Vec3& objectPosition, const Vec3& cameraPosition,
                                      const Vec3& cameraUpVector, const Vec3* cameraForwardVector,
                                      Mat4* dst);
};

inline Vec3& operator*=(Vec3& v, const Mat4& m);

inline Vec3 operator*(const Mat4& m, const Vec3& v);

inline Vec4& operator*=(Vec4& v, const Mat4& m);

inline Vec4 operator*(const Mat4& m, const Vec4& v);

MATH_END

#include "math/Mat4.inl"


#endif /* Mat4_hpp */
