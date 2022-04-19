//
//  Material.hpp
//  april3d
//
//  Created by 黄世平 on 2022/4/4.
//

#ifndef Material_hpp
#define Material_hpp

#include <stdio.h>
#include <string>
#include <unordered_map>

#include "renderer/RenderState.h"
#include "renderer/Technique.h"
#include "renderer/CustomCommand.h"
#include "base/Ref.h"
#include "base/Vector.h"
#include "platform/AprilPlatformMacros.h"


APRIL_BEGIN

class Technique;
class Pass;
class GLProgramState;
class Node;
class Properties;
class RenderState;

namespace backend {
    class ProgramState;
}

/**
 *材质系统
 */
class  Material :public Ref {
    friend class Node;
    friend class Technique;
    friend class Pass;
    friend class MeshCommand;
    friend class Renderer;
    friend class Mesh;
    friend class RenderState;

public:
  
    static Material* createWithFilename(const std::string& path);

    static Material* createWithProgramState(backend::ProgramState* programState);

    static Material* createWithProperties(Properties* materialProperties);

    void draw(MeshCommand* meshCommand, float globalZOrder, backend::Buffer* vertexBuffer, backend::Buffer* indexBuffer,
              CustomCommand::PrimitiveType primitive, CustomCommand::IndexFormat indexFormat,
              unsigned int indexCount, const Mat4& modelView);

    std::string getName() const;

    void setName(const std::string& name);

    Technique* getTechniqueByName(const std::string& name);

    Technique* getTechniqueByIndex(ssize_t index);

    Technique* getTechnique() const;

    const Vector<Technique*>& getTechniques() const;

    ssize_t getTechniqueCount() const;

    void addTechnique(Technique* technique);

    void setTechnique(const std::string& techniqueName);

    virtual Material* clone() const;

    inline RenderState::StateBlock &getStateBlock() { return _renderState._state; }

    inline void setStateBlock(const RenderState::StateBlock &state) {
        render_state.state_ = state;
    }

    RenderState * getRenderState() { return &render_state; }

protected:
    Material();
    ~Material();
    bool initWithProgramState(backend::ProgramState* state);
    bool initWithFile(const std::string& file);
    bool initWithProperties(Properties* materialProperties);

    void setTarget(Node* target);

    bool parseProperties(Properties* properties);
    bool parseTechnique(Properties* properties);
    bool parsePass(Technique* technique, Properties* properties);
    bool parseShader(Pass* pass, Properties* properties);
    bool parseSampler(backend::ProgramState* programState, Properties* properties);
    bool parseUniform(backend::ProgramState* programState, Properties* properties, const char* uniformName);
    bool parseRenderState(RenderState::StateBlock *state, Properties* properties);


    std::string name_;

    RenderState render_state;

    Vector<Technique*> techniques_;

    /**
     *weak pointer since it is being help by techniques_
     */
    Technique* current_technique = nullptr;

    /**
     *weak reference
     */
    Node* target_ = nullptr;

    std::unordered_map<std::string, int> texture_slots;
    int texture_slot_index = 0;
};

APRIL_END


#endif /* Material_hpp */
