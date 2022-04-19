//
//  Node.hpp
//  april3d
//
//  Created by 黄世平 on 2022/4/6.
//

#ifndef Node_hpp
#define Node_hpp

#include <stdio.h>
#include <cstdint>
#include <vector>

#include "base/aprilMacros.h"
#include "base/aprilTypes.h"
#include "math/AffineTransform.h"
#include "math/Math.h"

APRIL_BEGIN

class Scene;
class Renderer;
class Director;
class Material;
class Camera;

namespace backend{
    class ProgramState;
}

/**
 *1、Node在场景中的关系
 *2、Node拥有一系列转换操作
 *3、Node绘制
 */
class Node : public Ref {
public:

    static const int INVALID_TAG = -1;

    enum {
        FLAGS_TRANSFORM_DIRTY = (1 << 0),
        FLAGS_CONTENT_SIZE_DIRTY = (1 << 1),
        FLAGS_RENDER_AS_3D = (1 << 3),

        FLAGS_DIRTY_MASK = (FLAGS_TRANSFORM_DIRTY | FLAGS_CONTENT_SIZE_DIRTY),
    };

    static Node * create();

    static int getAttachedNodeCount();
public:
    
    virtual std::string getDescription() const;

    virtual void setLocalZOrder(std::int32_t localZOrder);
    
    virtual void _setLocalZOrder(std::int32_t z);

    void updateOrderOfArrival();

    virtual void setGlobalZOrder(float globalZOrder);

    virtual float getGlobalZOrder() const { return global_z_order; }

    virtual void setPosition(float x, float y);
  
    virtual void getPosition(float* x, float* y) const;
    
    virtual void setPositionZ(float positionZ);
 
    virtual void setPosition3D(const Vec3& position);

    virtual Vec3 getPosition3D() const;


    virtual void setContentSize(const Size& contentSize);
  
    virtual const Size& getContentSize() const;

    virtual void setVisible(bool visible);
  
    virtual bool isVisible() const;

    
    virtual void addChild(Node * child);
    
    virtual void removeAllChildren();

    virtual std::vector<Node*>& getChildren() { return children_; }
    
    virtual const std::vector<Node*>& getChildren() const { return children_; }
  
    virtual ssize_t getChildrenCount() const;

    virtual void setParent(Node* parent);
  
    virtual Node* getParent() { return parent_; }
    
    virtual const Node* getParent() const { return parent_; }
 
    
  
    virtual const std::string& getName() const;
   
    virtual void setName(const std::string& name);

    virtual bool isRunning() const;

    virtual void cleanup();

    virtual void draw(Renderer *renderer, const Mat4& transform, uint32_t flags);
    
    virtual void draw() final;

    virtual void visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags);
    
    virtual void visit() final;

    virtual Scene* getScene() const;

    virtual Rect getBoundingBox() const;
  
    virtual void update(float delta);

    virtual void updateTransform();

    /**
     *返回将节点的(局部)空间坐标转换为父节点空间坐标的矩阵
     */
    virtual const Mat4& getNodeToParentTransform() const;
    
    virtual AffineTransform getNodeToParentAffineTransform() const;

    virtual Mat4 getNodeToParentTransform(Node* ancestor) const;

    virtual AffineTransform getNodeToParentAffineTransform(Node* ancestor) const;

    virtual void setNodeToParentTransform(const Mat4& transform);

    virtual const Mat4& getParentToNodeTransform() const;
    
    virtual AffineTransform getParentToNodeAffineTransform() const;

    virtual Mat4 getNodeToWorldTransform() const;
    
    virtual AffineTransform getNodeToWorldAffineTransform() const;

    virtual Mat4 getWorldToNodeTransform() const;
    
    virtual AffineTransform getWorldToNodeAffineTransform() const;

    unsigned short getCameraMask() const { return camera_mask; }
   
    virtual void setCameraMask(unsigned short mask, bool applyChildren = true);
    
    /**
     *绘制相关
     */
    virtual void setProgramState(backend::ProgramState* programState);
    
    virtual backend::ProgramState* getProgramState() const;


    Node();
    virtual ~Node();

    virtual bool init();

    /**
     *检查当前来访的摄像机是否可见此摄像机遮罩
     */
    bool isVisitableByVisitingCamera() const;
    
protected:

    /**
     *变换相关
     */
    Vec3 position_;
    /**
     *OpenGL real Z position
     */
    float position_z;
    
    float rotation_x;
    
    float rotation_y;

    float scale_x;
    
    float scale_y;
    
    float scale_z;

    Size content_size;
    
    bool is_content_size_dirty;

    /**
     *ModelView transform of the Node.
     */
    Mat4 model_view_transform;

    mutable Mat4 transform_matrix;
    
    mutable bool is_transform_dirty;
    
    mutable Mat4 inverse_transform;
    
    mutable bool is_inverse_transform_dirty;
    
    mutable Mat4* additional_transform;
    
    mutable bool is_additional_transform_dirty;
    
    bool is_transform_updated;

    /**
     *全局Z
     */
    float global_z_order;

    static std::uint32_t s_global_order_of_arrival;

    std::vector<Node*> children_;
    
    Node* parent_;
    
    Director* director_;
    
    int tag_;
    
    std::string name_;
  
    bool is_running;

    bool is_visible;

    /**
     *摄像头掩模，只有当camera_mask和当前摄像头的摄像头标志为真时才可见
     */
    unsigned short camera_mask;
    
    backend::ProgramState* program_state = nullptr;

    static int attached_node_counts;
    
private:
    DISALLOW_COPY_AND_ASSIGN(Node);
};

APRIL_END

#endif /* Node_hpp */
