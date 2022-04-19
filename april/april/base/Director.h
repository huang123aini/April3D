//
//  Director.hpp
//  april3d
//
//  Created by 黄世平 on 2022/4/15.
//

#ifndef Director_hpp
#define Director_hpp

#include <stdio.h>
#include <stack>
#include <thread>
#include <chrono>
#include <vector>
#include <memory>

#include "platform/AprilPlatformMacros.h"
#include "base/Ref.h"
#include "scene/Scene.h"
#include "math/Math.h"

APRIL_BEGIN
class Node;
class Renderer;
class Camera;

class Director : public Ref {
public:

    enum class Projection {
        MODEL_2D,
        MODEL_3D,
        MODEL_CUSTOM,
        MODEL_DEFAULT = MODEL_3D,
    };
 
    static Director* getInstance();

    Director();
    
    ~Director();
    
    bool init();

    Scene* getRunningScene() { return running_scene; }
    
    Renderer* getRenderer() { return renderer_; }

    Projection getProjection() { return projection_model; }
    
    Mat4 getProjectionMatrix() { return projection_matrix; }

    void setProjection(Projection projection);
    
    void setViewport();
    
    float getZEye() const;

    void drawScene();

    inline void setWinSize(Size size) { win_size_in_points = size; }

    void setGLDefaultValues();
    
    void setClearColor(const Color4F& clearColor);
 
    void update(float dt);

    const std::thread::id& getApril3DThreadId() const { return april3d_thread_id; }

    bool isValid() const { return !is_invalid; }

protected:
    
    Scene* running_scene = nullptr;
    
    Renderer* renderer_ = nullptr;

    Projection projection_model = Projection::MODEL_DEFAULT;
    
    Mat4 projection_matrix = nullptr;

    Size win_size_in_points = Size::ZERO;
    
    float content_scale_factor = 1.0f;

    Color4F clear_color = {0, 0, 0, 1};

    std::thread::id april3d_thread_id;

    bool is_invalid = false;
};

APRIL_END


#endif /* Director_hpp */
