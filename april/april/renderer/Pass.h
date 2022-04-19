//
//  Pass.hpp
//  april3d
//
//  Created by 黄世平 on 2022/4/4.
//

#ifndef Pass_hpp
#define Pass_hpp

#include <stdio.h>

#include "platform/AprilPlatformMacros.h"
#include "renderer/RenderState.h"
#include "renderer/MeshCommand.h"
#include "renderer/GroupCommand.h"
#include "renderer/CallbackCommand.h"

APRIL_BEGIN

class Technique;
class Node;
class VertexAttribBinding;
class MeshIndexData;
class RenderState;

namespace backend {
    class ProgramState;
    class Buffer;
}

class Pass : public Ref {
    friend class Material;
    friend class Technique;
    friend class RenderState;
    friend class VertexAttribBinding;
public:

    static Pass* createWithProgramState(Technique* parent, backend::ProgramState* programState);
    
    static Pass* create(Technique* parent);

    backend::ProgramState* getProgramState() const;

    void draw(MeshCommand *meshCommand, float globalZOrder, backend::Buffer* vertexBuffer, backend::Buffer* indexBuffer,
              MeshCommand::PrimitiveType primitive, MeshCommand::IndexFormat indexFormat,
              unsigned int indexCount, const Mat4& modelView);


    void setVertexAttribBinding(VertexAttribBinding* binding);

    VertexAttribBinding* getVertexAttributeBinding() const;

    void setName(const std::string &name) { name_ = name; }
    const std::string &getName() const { return name_; }

    inline RenderState::StateBlock &getStateBlock() { return render_state.state_; }

    Pass* clone() const;

    void setTechnique(Technique *technique);

    void updateMVPUniform(const Mat4& modelView);
    
    void setUniformTexture(uint32_t slot, backend::TextureBackend *);      //u_texture
    void setUniformNormTexture(uint32_t slot, backend::TextureBackend *);  //u_texture

    void setUniformColor(const void *, size_t);                 //ucolor
    void setUniformMatrixPalette(const void *, size_t);         //u_matrixPalette

    void setUniformDirLightColor(const void *, size_t);
    void setUniformDirLightDir(const void *, size_t);

    void setUniformPointLightColor(const void *, size_t);
    void setUniformPointLightPosition(const void *, size_t);
    void setUniformPointLightRangeInverse(const void *, size_t);

    void setUniformSpotLightColor(const void *, size_t);
    void setUniformSpotLightPosition(const void *, size_t);
    void setUniformSpotLightDir(const void *, size_t);
    void setUniformSpotLightInnerAngleCos(const void *, size_t);
    void setUniformSpotLightOuterAngleCos(const void *, size_t);
    void setUniformSpotLightRangeInverse(const void *, size_t);

    void setUniformAmbientLigthColor(const void *, size_t);

protected:
    Pass();
    ~Pass();
    bool init(Technique* parent);
    bool initWithProgramState(Technique* parent, backend::ProgramState *glProgramState);

    void setProgramState(backend::ProgramState* programState);
    Node* getTarget() const;

    VertexAttribBinding*        vertex_attrib_binding    = nullptr;
    backend::ProgramState *     program_state           = nullptr;
    Technique *                 technique_             = nullptr;
    bool                        hash_dirty              = true;
    RenderState                 render_state;
    std::string                 name_;

private:
    
    void initUniformLocations();
    void onBeforeVisitCmd(MeshCommand *);
    void onAfterVisitCmd(MeshCommand *);

    backend::UniformLocation location_mvp_matrix;
    backend::UniformLocation location_mv_matrix;
    backend::UniformLocation location_p_matrix;
    backend::UniformLocation location_normal_matrix;

    backend::UniformLocation location_texture;               //u_texture
    backend::UniformLocation location_normal_texture;         //u_normalTex

    backend::UniformLocation location_color;                 //ucolor
    backend::UniformLocation location_matrix_palette;         //u_matrixPalette

    backend::UniformLocation location_dir_light_color;
    backend::UniformLocation location_dir_light_dir;

    backend::UniformLocation location_point_light_color;
    backend::UniformLocation location_point_light_position;
    backend::UniformLocation location_point_light_range_inverse;

    backend::UniformLocation location_spot_light_color;
    backend::UniformLocation location_spot_light_position;
    backend::UniformLocation location_spot_light_dir;
    backend::UniformLocation location_spot_light_inner_angle_cos;
    backend::UniformLocation location_spot_light_outer_angle_cos;
    backend::UniformLocation location_spot_light_range_inverse;

    backend::UniformLocation location_ambient_ligth_color;

    /**
     *renderer state cache variables
     */
    bool                        renderer_depth_test_enabled   = true;
    backend::CompareFunction    renderer_depth_cmp_func       = backend::CompareFunction::LESS;
    backend::CullMode           renderer_cull_mode           = backend::CullMode::BACK;
    backend::Winding            renderer_winding            = backend::Winding::COUNTER_CLOCK_WISE;
    bool                        renderer_depth_write         = false;
};

APRIL_END


#endif /* Pass_hpp */
