//
//  Camera.hpp
//  april3d
//
//  Created by 黄世平 on 2022/4/15.
//
//TODO:增加各种类型的摄像机

#ifndef Camera_hpp
#define Camera_hpp

#include <stdio.h>

#include "scene/Node.h"
#include "3d/CCFrustum.h"
#include "renderer/CCQuadCommand.h"
#include "renderer/CCCustomCommand.h"

APRIL_BEGIN

class Scene;
class CameraBackgroundBrush;

/**
 * 场景创建一个默认摄像头
 * 在渲染场景时，它绘制每个摄像头所看到的对象
 * 对于每个摄像机，透明3d精灵是在不透明3d精灵和其他2d对象之后渲染的
 */
enum class CameraFlag {
    DEFAULT = 1,
    USER1 = 1 << 1,
    USER2 = 1 << 2,
    USER3 = 1 << 3,
    USER4 = 1 << 4,
    USER5 = 1 << 5,
    USER6 = 1 << 6,
    USER7 = 1 << 7,
    USER8 = 1 << 8,
};

class Camera :public april3d::Node
{
    friend class Scene;
    friend class Director;
    friend class EventDispatcher;
public:
    enum class Type {
        PERSPECTIVE = 1,
        ORTHOGRAPHIC = 2
    };
public:
    static Camera* createPerspective(float fieldOfView, float aspectRatio, float nearPlane, float farPlane);
    
    static Camera* createOrthographic(float zoomX, float zoomY, float nearPlane, float farPlane);

    static Camera* create();

    /**
     * 获取当前正在访问的Camera
     */
    static const Camera* getVisitingCamera();

    static const Viewport& getDefaultViewport();
    
    static void setDefaultViewport(const Viewport& vp);

    static Camera* getDefaultCamera();

    Camera::Type getType() const { return type_; }

    CameraFlag getCameraFlag() const { return camera_flag; }
    
    void setCameraFlag(CameraFlag flag) { camera_flag = flag; }

    virtual void lookAt(const Vec3& target, const Vec3& up = Vec3::UNIT_Y);

    const Mat4& getProjectionMatrix() const;
   
    const Mat4& getViewMatrix() const;

    const Mat4& getViewProjectionMatrix() const;
    
    /*
     *将3D世界空间坐标中的指定点转换为屏幕空间坐标
     */
    Vec2 project(const Vec3& src) const;
    
    /*
     *将3D世界空间坐标中的指定点转换为gl屏幕空间坐标
     */
    Vec2 projectGL(const Vec3& src) const;
    
    /**
     *将指定的屏幕空间坐标点转换为3D世界空间坐标
     */
    Vec3 unproject(const Vec3& src) const;
    
    /**
     *将gl屏幕空间坐标的指定点转换为3D世界空间坐标
     */
    Vec3 unprojectGL(const Vec3& src) const;
    
    void unproject(const Size& size, const Vec3* src, Vec3* dst) const;
    
    void unprojectGL(const Size& size, const Vec3* src, Vec3* dst) const;


    bool isVisibleInFrustum(const AABB* aabb) const;
    
    /**
     *获取对象深度朝向相机
     */
    float getDepthInView(const Mat4& transform) const;
    
    void setDepth(int8_t depth);
    
    int8_t getDepth() const { return depth_; }
    
    int getRenderOrder() const;
    
    float getFarPlane() const { return far_plane; }

    float getNearPlane() const { return near_plane; }
    
    virtual void onEnter() override;
    
    virtual void onExit() override;

    void clearBackground();
    /**
     Apply the FBO, RenderTargets and viewport.
     */
    void apply();

    bool isViewProjectionUpdated() const {return view_projection_updated;}

    virtual void visit(Renderer* renderer, const Mat4 &parentTransform, uint32_t parentFlags) override;


    Camera();
    
    ~Camera();

    void setScene(Scene* scene);

    void setAdditionalProjection(const Mat4& mat);

    bool initDefault();
    
    bool initPerspective(float fieldOfView, float aspectRatio, float nearPlane, float farPlane);
    
    bool initOrthographic(float zoomX, float zoomY, float nearPlane, float farPlane);
    
    void applyViewport();

protected:
    static Camera* visiting_camera;
    static Viewport default_view_port;

    Scene* scene_ = nullptr;
    Mat4 projection_matrix;
    mutable Mat4 view_matrix;
    mutable Mat4 view_inverse_matrix;
    mutable Mat4 view_projection_matrix;

    Vec3 up_;
    
    Camera::Type type_;
    
    float field_of_view = 0.f;
    
    float zoom_[2] = {0.f};
    
    float aspect_ratio = 0.f;
    
    float near_plane = 0.f;
    
    float far_plane = 0.f;
    
    mutable bool  view_projection_dirty = true;
    /**
     *Whether or not the viewprojection matrix was updated since the last frame.
     */
    bool view_projection_updated = false;
    
    CameraFlag camera_flag = CameraFlag::DEFAULT;
    
    mutable Frustum frustum_;
    
    mutable bool frustum_Dirty = true;
    /**
     *flag默认为0，一个更大深度的相机被绘制在一个更小深度的相机的顶部
     */
    int8_t  depth_ = -1;
};

APRIL_END


#endif /* Camera_hpp */
