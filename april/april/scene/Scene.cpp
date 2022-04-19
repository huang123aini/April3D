//
//  Scene.cpp
//  april3d
//
//  Created by 黄世平 on 2022/4/15.
//

#include "Scene.h"

#include "scene/Scene.h"

#include "scene/Camera.h"

#include "renderer/Renderer.h"

APRIL_BEGIN

Scene::Scene()
: default_camera(Camera::create())  {
    addChild(default_camera);
}

Scene::~Scene() {}


bool Scene::init() {
    auto size = Director::getInstance()->getWinSize();
    return initWithSize(size);
}

bool Scene::initWithSize(const Size& size)
{
    setContentSize(size);
    return true;
}

Scene* Scene::create() {
    Scene *ret = new (std::nothrow) Scene();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    } else {
        SAFE_DELETE(ret);
        return nullptr;
    }
}

Scene* Scene::createWithSize(const Size& size) {
    Scene *ret = new (std::nothrow) Scene();
    if (ret && ret->initWithSize(size)) {
        ret->autorelease();
        return ret;
    } else {
        SAFE_DELETE(ret);
        return nullptr;
    }
}

std::string Scene::getDescription() const {
    /**return StringUtils::format("<Scene | tag = %d>", _tag);*/
    return nullptr;
}


void Scene::render(Renderer* renderer, const Mat4& eyeTransform, const Mat4* eyeProjection) {
    auto director = Director::getInstance();
   
    const auto& transform = getNodeToParentTransform();
    
    default_camera->apply();

    default_camera->clearBackground();

    visit(renderer, transform, 0);
    
    renderer->render();
}

void Scene::removeAllChildren() {
    if (default_camera) {
        default_camera->retain();
    }
        
    Node::removeAllChildren();

    if (default_camera) {
        addChild(default_camera);
        default_camera->release();
    }
}

APRIL_END

