//
//  Node.cpp
//  april3d
//
//  Created by 黄世平 on 2022/4/6.
//

#include "scene/Node.h"

#include <algorithm>
#include <string>
#include <regex>

#include "scene/Camera.h"
#include "scene/Scene.h"
#include "renderer/Material.h"
#include "math/TransformUtils.h"


#if CC_NODE_RENDER_SUBPIXEL
#define RENDER_IN_SUBPIXEL
#else
#define RENDER_IN_SUBPIXEL(__ARGS__) (ceil(__ARGS__))
#endif


APRIL_BEGIN

int Node::attached_node_counts = 0;

Node::Node()
: position_z(0.f)
, content_size(Size::ZERO)
, is_content_size_dirty(true)
, is_transform_dirty(true)
, is_inverse_transform_dirty(true)
, additional_transform(nullptr)
, is_additional_transform_dirty(false)
, global_z_order(0)
, parent_(nullptr)
, tag_(Node::INVALID_TAG)
, name_("")
, is_running(false)
, is_visible(true)
, camera_mask(1) {
    director_ = Director::getInstance();
    transform_matrix = inverse_matrix = Mat4::IDENTITY;
}

Node * Node::create() {
    Node * ret = new (std::nothrow) Node();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        SAFE_DELETE(ret);
    }
    return ret;
}

Node::~Node() {
    printf("deallocing Node: %p - tag: %i", this, tag_ );
    
    for (auto& child : children_) {
        child->parent_ = nullptr;
    }

    delete[] additional_transform;
    
    /**
     *删除绘制相关
     */
    SAFE_RELEASE(program_state);
}

bool Node::init()
{
    return true;
}

void Node::cleanup() {
    for( const auto &child: children_) {
        child->cleanup();
    }
}

std::string Node::getDescription() const {
    /**
     *TODO:
     *return StringUtils::format("<Node | Tag = %d", tag_);
     */
    return  nullptr;
    
}



void Node::setPosition(float x, float y) {
    if (position_.x == x && position_.y == y) {
        return;
    }
    position_.x = x;
    position_.y = y;
    is_transform_updated = is_transform_dirty = is_inverse_transform_dirty = true;
}

void Node::setPosition3D(const Vec3& position) {
    setPositionZ(position.z);
    setPosition(position.x, position.y);
}

Vec3 Node::getPosition3D() const {
    return Vec3(position_.x, position_.y, position_z);
}

void Node::setPositionZ(float positionZ) {
    if (position_z == positionZ) {
        return;
    }
    is_transform_updated = is_transform_dirty = is_inverse_transform_dirty = true;
    position_z = positionZ;
}


ssize_t Node::getChildrenCount() const {
    return children_.size();
}

bool Node::isVisible() const {
    return is_visible;
}

void Node::setVisible(bool visible) {
    if(visible != is_visible) {
        is_visible = visible;
        if(is_visible) {
            is_transform_updated = is_transform_dirty = is_inverse_transform_dirty = true;
        }
    }
}

const Size& Node::getContentSize() const {
    return content_size;
}

void Node::setContentSize(const Size & size) {
    if (! size.equals(content_size)) {
        content_size = size;
       is_transform_updated = is_transform_dirty = is_inverse_transform_dirty = is_content_size_dirty = true;
    }
}

bool Node::isRunning() const {
    return is_running;
}


Scene* Node::getScene() const {
    if (!parent_) {
        return nullptr;
    }
    auto sceneNode = parent_;
    while (sceneNode->parent_) {
        sceneNode = sceneNode->parent_;
    }
    return dynamic_cast<Scene*>(sceneNode);
}

Rect Node::getBoundingBox() const {
    Rect rect(0, 0, content_size.width, content_size.height);
    return RectApplyAffineTransform(rect, getNodeToParentAffineTransform());
}


void Node::addChild(Node *child) {
    children_.push_back(child);
}

void Node::draw() {
    auto renderer = director_->getRenderer();
    draw(renderer, model_view_transform, FLAGS_TRANSFORM_DIRTY);
}

void Node::draw(Renderer* /*renderer*/, const Mat4 & /*transform*/, uint32_t /*flags*/)
{}

void Node::visit() {
    auto renderer = director_->getRenderer();
    auto& parentTransform = director_->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    visit(renderer, parentTransform, FLAGS_TRANSFORM_DIRTY);
}

bool Node::isVisitableByVisitingCamera() const
{
    auto camera = Camera::getVisitingCamera();
    bool visibleByCamera = camera ? ((unsigned short)camera->getCameraFlag() & camera_mask) != 0 : true;
    return visibleByCamera;
}

void Node::visit(Renderer* renderer, const Mat4 &parentTransform, uint32_t parentFlags) {
    if (!is_visible) {
        return;
    }

    uint32_t flags = 1;

   
    bool visibleByCamera = isVisitableByVisitingCamera();

    int i = 0;

    if(!children_.empty()) {
        this->draw(renderer, model_view_transform, flags);
    }
}

/**
 *变换相关
 */
AffineTransform Node::getNodeToParentAffineTransform() const {
    AffineTransform ret;
    GLToCGAffine(getNodeToParentTransform().m, &ret);
    return ret;
}


Mat4 Node::getNodeToParentTransform(Node* ancestor) const {
    Mat4 t(this->getNodeToParentTransform());

    for (Node *p = parent_;  p != nullptr && p != ancestor ; p = p->getParent()) {
        t = p->getNodeToParentTransform() * t;
    }

    return t;
}

AffineTransform Node::getNodeToParentAffineTransform(Node* ancestor) const {
    AffineTransform t(this->getNodeToParentAffineTransform());

    for (Node *p = parent_; p != nullptr && p != ancestor; p = p->getParent())
        t = AffineTransformConcat(t, p->getNodeToParentAffineTransform());

    return t;
}
const Mat4& Node::getNodeToParentTransform() const {
    if (is_transform_dirty) {
        /**
         *translation
         */
        float x = position_.x;
        float y = position_.y;
        float z = position_z;
        
        /**
         *Transform Matrix = translation * rotation * scale
         */
        Mat4 translation;
        
        Mat4::createTranslation(x, y, z, &translation);
        
        //TODO:临时只实现了位置
        transform_matrix = translation;
        
        is_transform_dirty = false;
        
        return transform_matrix;
    }

}

AffineTransform Node::getNodeToWorldAffineTransform() const
{
    return this->getNodeToParentAffineTransform(nullptr);
}

Mat4 Node::getNodeToWorldTransform() const
{
    return this->getNodeToParentTransform(nullptr);
}

AffineTransform Node::getWorldToNodeAffineTransform() const
{
    return AffineTransformInvert(this->getNodeToWorldAffineTransform());
}

Mat4 Node::getWorldToNodeTransform() const
{
    return getNodeToWorldTransform().getInversed();
}


void Node::updateTransform() {
    for( const auto &child: children_)
        child->updateTransform();
}

void Node::setCameraMask(unsigned short mask, bool applyChildren) {
    camera_mask = mask;
    if (applyChildren) {
        for (const auto& child : children_) {
            child->setCameraMask(mask, applyChildren);
        }
    }
}

int Node::getAttachedNodeCount() {
    return attached_node_counts;
}

void Node::setProgramState(backend::ProgramState* programState) {
    if (program_state != programState) {
        SAFE_RELEASE(program_state);
        program_state = programState;
        SAFE_RETAIN(programState);
    }
}

backend::ProgramState* Node::getProgramState() const {
    return program_state;
}

APRIL_END

