//
//  Director.cpp
//  april3d
//
//  Created by 黄世平 on 2022/4/15.
//

#include "base/Director.h"
#include "renderer/Renderer.h"
#include "renderer/RenderState.h"
#include "scene/Camera.h"
#include "base/aprilMacros.h"
#include "base/AutoreleasePool.h"
#include "renderer/backend/ProgramCache.h"

APRIL_BEGIN

static Director *s_shared_director = nullptr;

#define DEFAULT_FPS        60
extern const char* cocos2dVersion();


Director* Director::getInstance() {
    if (!s_shared_director) {
        s_shared_director = new (std::nothrow) Director;
        s_shared_director->init();
    }

    return s_shared_director;
}

Director::Director() {}

bool Director::init() {
    renderer_ = new Renderer();
    setGLDefaultValues();
    return true;
}

Director::~Director() {
    SAFE_RELEASE(running_scene);
    if (renderer_) {
        delete renderer_;
    }
  
    s_shared_director = nullptr;

#if (APRIL_OS == APRIL_OS_IOS)
    exit(0);
#endif
}

void Director::setGLDefaultValues() {
    renderer_->setDepthTest(false);
    renderer_->setDepthCompareFunction(backend::CompareFunction::LESS_EQUAL);
    setProjection(projection_model);
}

void Director::drawScene() {
    /**
     *开始当前帧绘制
     */
    renderer_->beginFrame();
    
    renderer_->clear(ClearFlag::ALL, clear_color, 1, 0, -10000.0);
    
    if (running_scene) {
        renderer_->clearDrawStats();
    }
   /**
    *绘制
    */
   renderer_->render();
    
   /**
    *结束当前帧绘制
    */
   renderer_->endFrame();

}

void Director::setViewport() {
    renderer_->setViewPort(0, 0, win_size_in_points.width, win_size_in_points.height);
}

void Director::setProjection(Projection projection) {
    Size size = win_size_in_points;

    if (size.width == 0 || size.height == 0) {
        printf("april3d: warning, Director::setProjection() failed because size is 0");
        return;
    }

    setViewport();

    switch (projection) {
        case Projection::MODEL_2D: {
            Mat4 orthoMatrix;
            Mat4::createOrthographicOffCenter(0, size.width, 0, size.height, -1024, 1024, &orthoMatrix);
            projection_matrix = orthoMatrix;
            break;
        }
            
        case Projection::MODEL_3D: {
            float zeye = this->getZEye();

            Mat4 matrixPerspective, matrixLookup;
            Mat4::createPerspective(60, (float)size.width/size.height, 10, zeye+size.height/2, &matrixPerspective);

            Vec3 eye(size.width/2, size.height/2, zeye), center(size.width/2, size.height/2, 0.0f), up(0.0f, 1.0f, 0.0f);
            Mat4::createLookAt(eye, center, up, &matrixLookup);
            Mat4 proj3d = matrixPerspective * matrixLookup;
            projection_matrix = proj3d;
            break;
        }

        case Projection::MODEL_CUSTOM:
            break;

        default:
            printf("april3d: Director: unrecognized projection \n");
            break;
    }

    projection_model = projection;

}

float Director::getZEye() const {
    return (win_size_in_points.height / 1.154700538379252f);//(2 * tanf(M_PI/6))
}

void Director::update(float dt) {
    if (! is_invalid) {
         drawScene();
         /**
          *自动释放池对象释放
          */
         PoolManager::getInstance()->getCurrentPool()->clear();
     }
}

APRIL_END


