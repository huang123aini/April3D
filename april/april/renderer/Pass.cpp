//
//  Pass.cpp
//  april3d
//
//  Created by 黄世平 on 2022/4/4.
//

#include "renderer/Pass.h"
#include <xxhash.h>
#include "renderer/Texture2D.h"
#include "renderer/Technique.h"
#include "renderer/Material.h"
#include "renderer/backend/ProgramState.h"
#include "3d/CCMeshVertexIndexData.h"
#include "3d/CCVertexAttribBinding.h"
#include "base/CCDirector.h"
#include "renderer/CCRenderer.h"

#include "base/aprilTypes.h"
#include "base/ccUtils.h"
#include "2d/Node.h"

APRIL_BEGIN

static const char          *s_dirLightUniformColorName = "u_DirLightSourceColor";
static const char          *s_dirLightUniformDirName = "u_DirLightSourceDirection";

static const char          *s_pointLightUniformColorName = "u_PointLightSourceColor";
static const char          *s_pointLightUniformPositionName = "u_PointLightSourcePosition";
static const char          *s_pointLightUniformRangeInverseName = "u_PointLightSourceRangeInverse";

static const char          *s_spotLightUniformColorName = "u_SpotLightSourceColor";
static const char          *s_spotLightUniformPositionName = "u_SpotLightSourcePosition";
static const char          *s_spotLightUniformDirName = "u_SpotLightSourceDirection";
static const char          *s_spotLightUniformInnerAngleCosName = "u_SpotLightSourceInnerAngleCos";
static const char          *s_spotLightUniformOuterAngleCosName = "u_SpotLightSourceOuterAngleCos";
static const char          *s_spotLightUniformRangeInverseName = "u_SpotLightSourceRangeInverse";

static const char          *s_ambientLightUniformColorName = "u_AmbientLightSourceColor";

Pass* Pass::create(Technique* technique) {
    auto pass = new (std::nothrow) Pass();
    if (pass && pass->init(technique)) {
        pass->autorelease();
        return pass;
    }
    SAFE_DELETE(pass);
    return nullptr;
}

Pass* Pass::createWithProgramState(Technique* technique, backend::ProgramState* programState) {
    auto pass = new (std::nothrow) Pass();
    if (pass && pass->initWithProgramState(technique, programState)) {
        pass->autorelease();
        return pass;
    }
    SAFE_DELETE(pass);
    return nullptr;
}

bool Pass::init(Technique* technique) {
    technique_ = technique;
    return true;
}

bool Pass::initWithProgramState(Technique* technique, backend::ProgramState *programState) {
    technique_ = technique;
    setProgramState(programState);
    return true;
}

Pass::Pass() {}

Pass::~Pass() {
    SAFE_RELEASE(vertex_attrib_binding);
    SAFE_RELEASE(program_state);
}

Pass* Pass::clone() const {
    auto pass = new (std::nothrow) Pass();
    if (pass) {
        pass->render_state = render_state;
        pass->setProgramState(program_state->clone());
        pass->vertex_attrib_binding = vertex_attrib_binding;
        SAFE_RETAIN(pass->vertex_attrib_binding);
        pass->setTechnique(technique_);
        pass->autorelease();
    }
    return pass;
}

backend::ProgramState* Pass::getProgramState() const {
   return program_state;
}

void Pass::setProgramState(backend::ProgramState* programState) {
    if (program_state != programState) {
        SAFE_RELEASE(program_state);
        program_state = programState;
        SAFE_RETAIN(program_state);
        initUniformLocations();
        hash_dirty = true;
    }
}

void Pass::initUniformLocations() {
    auto *ps = _programState;

    location_mvp_matrix = ps->getUniformLocation("u_MVPMatrix");
    location_mv_matrix = ps->getUniformLocation("u_MVMatrix");
    location_p_matrix = ps->getUniformLocation("u_PMatrix");
    location_normal_matrix = ps->getUniformLocation("u_NormalMatrix");

    location_texture = ps->getUniformLocation("u_texture");
    location_normal_texture = ps->getUniformLocation("u_normalTex");
    
    location_color = ps->getUniformLocation("u_color");
    location_matrix_palette = ps->getUniformLocation("u_matrixPalette");

    location_dir_light_color = ps->getUniformLocation(s_dirLightUniformColorName);
    location_dir_light_dir = ps->getUniformLocation(s_dirLightUniformDirName);

    location_point_light_color = ps->getUniformLocation(s_pointLightUniformColorName);
    location_point_light_position = ps->getUniformLocation(s_pointLightUniformPositionName);
    location_point_light_range_inverse = ps->getUniformLocation(s_pointLightUniformRangeInverseName);

    location_spot_light_color = ps->getUniformLocation(s_spotLightUniformColorName);
    location_spot_light_position = ps->getUniformLocation(s_spotLightUniformPositionName);
    location_spot_light_dir = ps->getUniformLocation(s_spotLightUniformDirName);
    location_spot_light_inner_angle_cos = ps->getUniformLocation(s_spotLightUniformInnerAngleCosName);
    location_spot_light_outer_angle_cos = ps->getUniformLocation(s_spotLightUniformOuterAngleCosName);
    location_spot_light_range_inverse = ps->getUniformLocation(s_spotLightUniformRangeInverseName);

    location_ambient_ligth_color = ps->getUniformLocation(s_ambientLightUniformColorName);
}

void Pass::draw(MeshCommand *meshCommand, float globalZOrder, backend::Buffer* vertexBuffer, backend::Buffer* indexBuffer,
                MeshCommand::PrimitiveType primitive, MeshCommand::IndexFormat indexFormat,
                unsigned int indexCount, const Mat4& modelView) {

    meshCommand->setBeforeCallback(CC_CALLBACK_0(Pass::onBeforeVisitCmd, this, meshCommand));
    meshCommand->setAfterCallback(CC_CALLBACK_0(Pass::onAfterVisitCmd, this, meshCommand));
    meshCommand->init(globalZOrder, modelView);
    meshCommand->setPrimitiveType(primitive);
    meshCommand->setIndexBuffer(indexBuffer, indexFormat);
    meshCommand->setVertexBuffer(vertexBuffer);
    meshCommand->setIndexDrawInfo(0, indexCount);
    meshCommand->getPipelineDescriptor().programState = _programState;


    auto *renderer = Director::getInstance()->getRenderer();

    renderer->addCommand(meshCommand);
}

void Pass::updateMVPUniform(const Mat4& modelView) {
    auto &matrixP = Director::getInstance()->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    auto mvp = matrixP * modelView;
    program_state->setUniform(location_mvp_matrix, mvp.m, sizeof(mvp.m));
    if (location_mv_matrix) {
        program_state->setUniform(location_mv_matrix, modelView.m, sizeof(modelView.m));
    }
    if (location_p_matrix) {
        program_state->setUniform(location_p_matrix, matrixP.m, sizeof(matrixP.m));
    }
    if (location_normal_matrix) {
        auto normalMatrix = utils::getNormalMat3OfMat4(modelView);
        program_state->setUniform(location_normal_matrix, normalMatrix.data(), sizeof(normalMatrix[0]) * normalMatrix.size());
    }
}

void Pass::onBeforeVisitCmd(MeshCommand *command) {
    auto *renderer = Director::getInstance()->getRenderer();

    renderer_depth_test_enabled = renderer->getDepthTest();
    renderer_depth_cmp_func = renderer->getDepthCompareFunction();
    renderer_cull_mode = renderer->getCullMode();
    renderer_depth_write = renderer->getDepthWrite();
    renderer_winding = renderer->getWinding();
    /**
     *apply state blocks
     */
    render_state.bindPass(this, command);

    updateMVPUniform(command->getMV());
}

void Pass::onAfterVisitCmd(MeshCommand *command) {
    auto *renderer = Director::getInstance()->getRenderer();
    /**
     *restore renderer states
     */
    renderer->setDepthTest(renderer_depth_test_enabled);
    renderer->setDepthCompareFunction(renderer_depth_cmp_func);
    renderer->setCullMode(renderer_cull_mode);
    renderer->setDepthWrite(renderer_depth_write);
    renderer->setWinding(renderer_winding);
}


Node* Pass::getTarget() const {
    
    if (!technique || !technique->material_) {
        printf("Pass must have a Technique and Material. \n");
        return nullptr;
    }
    Material *material = technique_->material_;
    return material->target_;
}

void Pass::setTechnique(Technique *technique) {
    technique_ = technique; //weak reference
}

void Pass::setVertexAttribBinding(VertexAttribBinding* binding) {
    if (vertex_attrib_binding != binding) {
        SAFE_RELEASE(vertex_attrib_binding);
        vertex_attrib_binding = binding;
        SAFE_RETAIN(vertex_attrib_binding);
    }
}

VertexAttribBinding* Pass::getVertexAttributeBinding() const {
    return vertex_attrib_binding;
}

void Pass::setUniformTexture(uint32_t slot, backend::TextureBackend *tex) {
    program_state->setTexture(location_texture, slot, tex);
}

void Pass::setUniformNormTexture(uint32_t slot, backend::TextureBackend *tex) {
    program_state->setTexture(location_normal_texture, slot, tex);
}

#define TRY_SET_UNIFORM(location) do {                                   \
    if(location) {                                                       \
        program_state->setUniform(location, data, (uint32_t)dataLen);    \
    } } while(false)


void Pass::setUniformColor(const void *data, size_t dataLen) {
    TRY_SET_UNIFORM(location_color);
}

void Pass::setUniformMatrixPalette(const void *data, size_t dataLen) {
    TRY_SET_UNIFORM(location_matrix_palette);
}


void Pass::setUniformDirLightColor(const void *data, size_t dataLen) {
    TRY_SET_UNIFORM(location_dir_light_color);
}

void Pass::setUniformDirLightDir(const void *data, size_t dataLen) {
    TRY_SET_UNIFORM(location_dir_light_dir);
}

void Pass::setUniformPointLightColor(const void *data, size_t dataLen) {
    TRY_SET_UNIFORM(location_point_light_color);
}

void Pass::setUniformPointLightPosition(const void *data, size_t dataLen) {
    TRY_SET_UNIFORM(location_point_light_position);
}

void Pass::setUniformPointLightRangeInverse(const void *data, size_t dataLen) {
    TRY_SET_UNIFORM(location_point_light_range_inverse);
}

void Pass::setUniformSpotLightColor(const void *data, size_t dataLen) {
    TRY_SET_UNIFORM(location_spot_light_color);
}

void Pass::setUniformSpotLightPosition(const void *data, size_t dataLen) {
    TRY_SET_UNIFORM(location_spot_light_position);
}

void Pass::setUniformSpotLightDir(const void *data, size_t dataLen) {
    TRY_SET_UNIFORM(location_spot_light_dir);
}

void Pass::setUniformSpotLightInnerAngleCos(const void *data, size_t dataLen) {
    TRY_SET_UNIFORM(location_spot_light_inner_angle_cos);
}

void Pass::setUniformSpotLightOuterAngleCos(const void *data, size_t dataLen) {
    TRY_SET_UNIFORM(location_spot_light_outer_angle_cos);
}

void Pass::setUniformSpotLightRangeInverse(const void *data, size_t dataLen) {
    TRY_SET_UNIFORM(location_spot_light_range_inverse);
}

void Pass::setUniformAmbientLigthColor(const void *data, size_t dataLen) {
    TRY_SET_UNIFORM(location_ambient_ligth_color);
}

APRIL_END
