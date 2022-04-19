//
//  Scene.hpp
//  april3d
//
//  Created by 黄世平 on 2022/4/15.
//

#ifndef Scene_hpp
#define Scene_hpp

#include <stdio.h>
#include <string>
#include "scene/Node.h"

APRIL_BEGIN

class Camera;
class BaseLight;
class Renderer;


class Scene : public Node {
public:

    static Scene *create();

    static Scene *createWithSize(const Size& size);

    using Node::addChild;
    
    virtual std::string getDescription() const override;
    
    const std::vector<Camera*>& getCameras();

    Camera* getDefaultCamera() const { return default_camera; }

    virtual void render(Renderer* renderer, const Mat4& eyeTransform, const Mat4* eyeProjection = nullptr);
  
    virtual void removeAllChildren() override;
    
    Scene();
    
    virtual ~Scene();
    
    bool init() override;
    
    bool initWithSize(const Size& size);
    
    void setCameraOrderDirty() { camera_order_dirty = true; }
    
protected:
    friend class Node;
    friend class Camera;
    friend class Renderer;
    Camera*              default_camera = nullptr;
    /**
     *order is dirty, need sort
     **/
    bool                 camera_order_dirty = true;
};


APRIL_END

#endif /* Scene_hpp */
