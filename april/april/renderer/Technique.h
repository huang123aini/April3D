//
//  Technique.hpp
//  april3d
//
//  Created by 黄世平 on 2022/4/4.
//

#ifndef Technique_hpp
#define Technique_hpp

#include <stdio.h>
#include <string>
#include "renderer/RenderState.h"
#include "base/Ref.h"
#include "platform/PlatformMacros.h"
#include "base/CCVector.h"

APRIL_BEGIN

class Pass;
class Material;

namespace  backend {
    class ProgramState;
}

class Technique :public Ref {
    friend class Material;
    friend class Renderer;
    friend class Pass;
    friend class MeshCommand;
    friend class Mesh;
    friend class RenderState;

public:

    static Technique* createWithProgramState(Material* parent, backend::ProgramState* state);
    static Technique* create(Material* parent);

    /**
     *顺序添加Pass,先添加的先绘制
     */
    void addPass(Pass* pass);

    std::string getName() const;
    
    Pass* getPassByIndex(ssize_t index) const;

    ssize_t getPassCount() const;

    const Vector<Pass*>& getPasses() const;

    Technique* clone() const;

    void setMaterial(Material * material) { material_ = material; }

    RenderState::StateBlock &getStateBlock() { return render_state.getStateBlock(); }

protected:
    Technique();
    ~Technique();
    bool init(Material* parent);

    void setName(const std::string& name);
    RenderState render_state;
    std::string name_;
    Vector<Pass*> passes_;

    Material* material_ = nullptr;
};

APRIL_END


#endif /* Technique_hpp */
