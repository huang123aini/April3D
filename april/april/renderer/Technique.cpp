//
//  Technique.cpp
//  april3d
//
//  Created by 黄世平 on 2022/4/4.
//

#include "renderer/Technique.h"
#include "renderer/Material.h"
#include "renderer/Pass.h"

APRIL_BEGIN

Technique* Technique::createWithProgramState(Material* parent, backend::ProgramState* state) {
    auto technique = new (std::nothrow) Technique();
    if (technique && technique->init(parent)) {
        auto pass = Pass::createWithProgramState(technique, state);
        technique->addPass(pass);
        technique->autorelease();
        return technique;
    }
    return  nullptr;
}

Technique* Technique::create(Material* material) {
    auto technique = new (std::nothrow) Technique();
    if (technique && technique->init(material)) {
        technique->autorelease();
        return technique;
    }
    return nullptr;
}

Technique::Technique()
: name_("") {}

Technique::~Technique(){}

bool Technique::init(Material* parent) {
    material_ = parent;
    return true;
}

Technique* Technique::clone() const {
    auto technique = new (std::nothrow) Technique();

    if (technique) {
        technique->name_ = name_;
        technique->render_state = render_state;
        for (const auto pass: passes_) {
            auto p = pass->clone();
            p->technique_ = technique;
            technique->passes_.pushBack(p);
        }
        technique->autorelease();
    }
    return technique;
}

void Technique::addPass(Pass *pass) {
    passes_.pushBack(pass);
}

std::string Technique::getName() const {
    return name_;
}

void Technique::setName(const std::string &name) {
    name_ = name;
}

Pass* Technique::getPassByIndex(ssize_t index) const {
    if (index < 0 || index >= passes_.size()) {
        printf("Technique::getPassByIndex Invalid Index. \n");
        return nullptr;
    }
    return passes_.at(index);
}

ssize_t Technique::getPassCount() const {
    return passes_.size();
}

const Vector<Pass*>& Technique::getPasses() const {
    return passes_;
}

APRIL_END
