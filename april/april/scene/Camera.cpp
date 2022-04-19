//
//  Camera.cpp
//  april3d
//
//  Created by 黄世平 on 2022/4/15.
//

#include "scene/Camera.h"
#include "base/Director.h"
#include "platform/GLView.h"
#include "renderer/Renderer.h"
#include "renderer/QuadCommand.h"


APRIL_BEGIN

Camera* Camera::visiting_camera = nullptr;
Viewport Camera::default_view_port;

Camera* Camera::create() {
    Camera* camera = new (std::nothrow) Camera();
    camera->initDefault();
    camera->autorelease();
    camera->setDepth(0);
    
    return camera;
}

Camera* Camera::createPerspective(float fieldOfView, float aspectRatio, float nearPlane, float farPlane) {
    auto ret = new (std::nothrow) Camera();
    if (ret) {
        ret->initPerspective(fieldOfView, aspectRatio, nearPlane, farPlane);
        ret->autorelease();
        return ret;
    }
    SAFE_DELETE(ret);
    return nullptr;
}

Camera* Camera::createOrthographic(float zoomX, float zoomY, float nearPlane, float farPlane) {
    auto ret = new (std::nothrow) Camera();
    if (ret) {
        ret->initOrthographic(zoomX, zoomY, nearPlane, farPlane);
        ret->autorelease();
        return ret;
    }
    SAFE_DELETE(ret);
    return nullptr;
}

Camera* Camera::getDefaultCamera() {
    auto scene = Director::getInstance()->getRunningScene();
    if(scene) {
        return scene->getDefaultCamera();
    }

    return nullptr;
}

const Viewport& Camera::getDefaultViewport()
{
    return default_view_port;
}
void Camera::setDefaultViewport(const Viewport& vp)
{
    default_view_port = vp;
}

const Camera* Camera::getVisitingCamera() {
    return visiting_camera;
}

Camera::Camera() {
}

Camera::~Camera() {
}

const Mat4& Camera::getProjectionMatrix() const
{
    return projection_matrix;
}
const Mat4& Camera::getViewMatrix() const {
    Mat4 viewInv(getNodeToWorldTransform());
    static int count = sizeof(float) * 16;
    if (memcmp(viewInv.m, _viewInv.m, count) != 0) {
        _viewProjectionDirty = true;
        _frustumDirty = true;
        _viewInv = viewInv;
        _view = viewInv.getInversed();
    }
    return _view;
}
void Camera::lookAt(const Vec3& lookAtPos, const Vec3& up) {
    Vec3 upv = up;
    upv.normalize();
    Vec3 zaxis;
    Vec3::subtract(this->getPosition3D(), lookAtPos, &zaxis);
    zaxis.normalize();
    
    Vec3 xaxis;
    Vec3::cross(upv, zaxis, &xaxis);
    xaxis.normalize();
    
    Vec3 yaxis;
    Vec3::cross(zaxis, xaxis, &yaxis);
    yaxis.normalize();
    Mat4  rotation;
    rotation.m[0] = xaxis.x;
    rotation.m[1] = xaxis.y;
    rotation.m[2] = xaxis.z;
    rotation.m[3] = 0;
    
    rotation.m[4] = yaxis.x;
    rotation.m[5] = yaxis.y;
    rotation.m[6] = yaxis.z;
    rotation.m[7] = 0;
    rotation.m[8] = zaxis.x;
    rotation.m[9] = zaxis.y;
    rotation.m[10] = zaxis.z;
    rotation.m[11] = 0;
    
    Quaternion  quaternion;
    Quaternion::createFromRotationMatrix(rotation,&quaternion);
    quaternion.normalize();
    setRotationQuat(quaternion);
}

const Mat4& Camera::getViewProjectionMatrix() const {
    getViewMatrix();
    if (_viewProjectionDirty) {
        _viewProjectionDirty = false;
        Mat4::multiply(projection_matrix, _view, &_viewProjection);
    }
    
    return _viewProjection;
}

void Camera::setAdditionalProjection(const Mat4& mat)
{
    projection_matrix = mat * projection_matrix;
    getViewProjectionMatrix();
}

bool Camera::initDefault() {
    auto size = Director::getInstance()->getWinSize();
    auto projection = Director::getInstance()->getProjection();
    switch (projection) {
        case Director::Projection::MODEL_2D: {
            initOrthographic(size.width, size.height, -1024, 1024);
            setPosition3D(Vec3(0.0f, 0.0f, 0.0f));
            setRotation3D(Vec3(0.f, 0.f, 0.f));
            break;
        }
        case Director::Projection::MODEL_3D: {
            float zeye = Director::getInstance()->getZEye();
            initPerspective(60, (float)size.width / size.height, 10, zeye + size.height / 2.0f);
            Vec3 eye(size.width/2, size.height/2.0f, zeye), center(size.width/2, size.height/2, 0.0f), up(0.0f, 1.0f, 0.0f);
            setPosition3D(eye);
            lookAt(center, up);
            break;
        }
        default:
            printf("Camera::initDefault unrecognized projection \n");
            break;
    }
    return true;
}

bool Camera::initPerspective(float fieldOfView, float aspectRatio, float nearPlane, float farPlane) {
    _fieldOfView = fieldOfView;
    _aspectRatio = aspectRatio;
    _nearPlane = nearPlane;
    _farPlane = farPlane;
    Mat4::createPerspective(_fieldOfView, _aspectRatio, _nearPlane, _farPlane, &projection_matrix);
    _viewProjectionDirty = true;
    _frustumDirty = true;
    _type = Type::PERSPECTIVE;
    
    return true;
}

bool Camera::initOrthographic(float zoomX, float zoomY, float nearPlane, float farPlane) {
    _zoom[0] = zoomX;
    _zoom[1] = zoomY;
    _nearPlane = nearPlane;
    _farPlane = farPlane;
    Mat4::createOrthographicOffCenter(0, _zoom[0], 0, _zoom[1], _nearPlane, _farPlane, &projection_matrix);
    _viewProjectionDirty = true;
    _frustumDirty = true;
    _type = Type::ORTHOGRAPHIC;
    
    return true;
}

Vec2 Camera::project(const Vec3& src) const
{
    Vec2 screenPos;
    
    auto viewport = Director::getInstance()->getWinSize();
    Vec4 clipPos;
    getViewProjectionMatrix().transformVector(Vec4(src.x, src.y, src.z, 1.0f), &clipPos);
    
    CCASSERT(clipPos.w != 0.0f, "clipPos.w can't be 0.0f!");
    float ndcX = clipPos.x / clipPos.w;
    float ndcY = clipPos.y / clipPos.w;
    
    screenPos.x = (ndcX + 1.0f) * 0.5f * viewport.width;
    screenPos.y = (1.0f - (ndcY + 1.0f) * 0.5f) * viewport.height;
    return screenPos;
}

Vec2 Camera::projectGL(const Vec3& src) const {
    Vec2 screenPos;
    
    auto viewport = Director::getInstance()->getWinSize();
    Vec4 clipPos;
    getViewProjectionMatrix().transformVector(Vec4(src.x, src.y, src.z, 1.0f), &clipPos);
    
    CCASSERT(clipPos.w != 0.0f, "clipPos.w can't be 0.0f!");
    float ndcX = clipPos.x / clipPos.w;
    float ndcY = clipPos.y / clipPos.w;
    
    screenPos.x = (ndcX + 1.0f) * 0.5f * viewport.width;
    screenPos.y = (ndcY + 1.0f) * 0.5f * viewport.height;
    return screenPos;
}

Vec3 Camera::unproject(const Vec3& src) const {
    Vec3 dst;
    unproject(Director::getInstance()->getWinSize(), &src, &dst);
    return dst;
}

Vec3 Camera::unprojectGL(const Vec3& src) const
{
    Vec3 dst;
    unprojectGL(Director::getInstance()->getWinSize(), &src, &dst);
    return dst;
}

void Camera::unproject(const Size& viewport, const Vec3* src, Vec3* dst) const
{
    CCASSERT(src && dst, "vec3 can not be null");
    
    Vec4 screen(src->x / viewport.width, ((viewport.height - src->y)) / viewport.height, src->z, 1.0f);
    screen.x = screen.x * 2.0f - 1.0f;
    screen.y = screen.y * 2.0f - 1.0f;
    screen.z = screen.z * 2.0f - 1.0f;
    
    getViewProjectionMatrix().getInversed().transformVector(screen, &screen);
    if (screen.w != 0.0f)
    {
        screen.x /= screen.w;
        screen.y /= screen.w;
        screen.z /= screen.w;
    }
    
    dst->set(screen.x, screen.y, screen.z);
}

void Camera::unprojectGL(const Size& viewport, const Vec3* src, Vec3* dst) const
{
    CCASSERT(src && dst, "vec3 can not be null");
    
    Vec4 screen(src->x / viewport.width, src->y / viewport.height, src->z, 1.0f);
    screen.x = screen.x * 2.0f - 1.0f;
    screen.y = screen.y * 2.0f - 1.0f;
    screen.z = screen.z * 2.0f - 1.0f;
    
    getViewProjectionMatrix().getInversed().transformVector(screen, &screen);
    if (screen.w != 0.0f)
    {
        screen.x /= screen.w;
        screen.y /= screen.w;
        screen.z /= screen.w;
    }
    
    dst->set(screen.x, screen.y, screen.z);
}

 bool Camera::isVisibleInFrustum(const AABB* aabb) const
 {
     if (_frustumDirty)
     {
         _frustum.initFrustum(this);
         _frustumDirty = false;
     }
     return !_frustum.isOutOfFrustum(*aabb);
 }

float Camera::getDepthInView(const Mat4& transform) const
{
    Mat4 camWorldMat = getNodeToWorldTransform();
    const Mat4 &viewMat = camWorldMat.getInversed();
    float depth = -(viewMat.m[2] * transform.m[12] + viewMat.m[6] * transform.m[13] + viewMat.m[10] * transform.m[14] + viewMat.m[14]);
    return depth;
}

void Camera::setDepth(int8_t depth)
{
    if (_depth != depth)
    {
        _depth = depth;
        if (_scene)
        {
            //notify scene that the camera order is dirty
            _scene->setCameraOrderDirty();
        }
    }
}

void Camera::setScene(Scene* scene)
{
    if (_scene != scene)
    {
        //remove old scene
        if (_scene)
        {
            auto& cameras = _scene->_cameras;
            auto it = std::find(cameras.begin(), cameras.end(), this);
            if (it != cameras.end())
                cameras.erase(it);
            _scene = nullptr;
        }
        //set new scene
        if (scene)
        {
            _scene = scene;
            auto& cameras = _scene->_cameras;
            auto it = std::find(cameras.begin(), cameras.end(), this);
            if (it == cameras.end())
            {
                _scene->_cameras.push_back(this);
                //notify scene that the camera order is dirty
                _scene->setCameraOrderDirty();
            }
        }
    }
}


void Camera::apply() {
    _viewProjectionUpdated = _transformUpdated;
    applyViewport();
}

void Camera::applyViewport() {
    Director::getInstance()->getRenderer()->setViewPort(default_view_port.x, default_view_port.y, default_view_port.w, default_view_port.h);
}

void Camera::visit(Renderer* renderer, const Mat4 &parentTransform, uint32_t parentFlags) {
    _viewProjectionUpdated = _transformUpdated;
    return Node::visit(renderer, parentTransform, parentFlags);
}

APRIL_END

