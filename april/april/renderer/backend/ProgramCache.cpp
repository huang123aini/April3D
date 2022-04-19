//
//  ProgramCache.cpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/2.
//

#include "ProgramCache.h"
#include "Device.h"
#include "ShaderModule.h"
#include "renderer/Shaders.h"
#include "base/aprilMacros.h"
#include "base/Configuration.h"

namespace std {
template <>
struct hash<backend::ProgramType> {
    typedef backend::ProgramType argument_type;
    typedef std::size_t result_type;
    result_type operator()(argument_type const& v) const {
        return hash<int>()(static_cast<int>(v));
    }
};
}

BACKEND_BEGIN

std::unordered_map<backend::ProgramType, backend::Program*>  ProgramCache::cached_programs;
ProgramCache* ProgramCache::s_shared_program_cache = nullptr;

ProgramCache* ProgramCache::getInstance() {
    if(!s_shared_program_cache) {
        s_shared_program_cache = new (std::nothrow) ProgramCache();
        if(!s_shared_program_cache->init()) {
            SAFE_RELEASE(s_shared_program_cache);
        }
    }
    return s_shared_program_cache;
}

void ProgramCache::destroyInstance() {
    SAFE_RELEASE_NULL(s_shared_program_cache);
}

ProgramCache::~ProgramCache() {
    for(auto& program : cached_programs) {
        SAFE_RELEASE(program.second);
    }
    printf("deallocing ProgramCache: %p \n", this);
    ShaderCache::destroyInstance();
}

bool ProgramCache::init() {
    /**
     *添加引擎内置Program
     */
    addProgram(ProgramType::POSITION_TEXTURE_COLOR);
    addProgram(ProgramType::ETC1);
    addProgram(ProgramType::LABEL_DISTANCE_NORMAL);
    addProgram(ProgramType::LABEL_NORMAL);
    addProgram(ProgramType::LABLE_OUTLINE);
    addProgram(ProgramType::LABLE_DISTANCEFIELD_GLOW);
    addProgram(ProgramType::POSITION_COLOR_LENGTH_TEXTURE);
    addProgram(ProgramType::POSITION_COLOR_TEXTURE_AS_POINTSIZE);
    addProgram(ProgramType::POSITION_COLOR);
    addProgram(ProgramType::POSITION);
    addProgram(ProgramType::LAYER_RADIA_GRADIENT);
    addProgram(ProgramType::POSITION_TEXTURE);
    addProgram(ProgramType::POSITION_TEXTURE_COLOR_ALPHA_TEST);
    addProgram(ProgramType::POSITION_UCOLOR);
    addProgram(ProgramType::ETC1_GRAY);
    addProgram(ProgramType::GRAY_SCALE);
    addProgram(ProgramType::LINE_COLOR_3D);
    addProgram(ProgramType::CAMERA_CLEAR);
    addProgram(ProgramType::SKYBOX_3D);
    addProgram(ProgramType::SKINPOSITION_TEXTURE_3D);
    addProgram(ProgramType::SKINPOSITION_NORMAL_TEXTURE_3D);
    addProgram(ProgramType::POSITION_NORMAL_TEXTURE_3D);
    addProgram(ProgramType::POSITION_TEXTURE_3D);
    addProgram(ProgramType::POSITION_3D);
    addProgram(ProgramType::POSITION_NORMAL_3D);
    addProgram(ProgramType::POSITION_BUMPEDNORMAL_TEXTURE_3D);
    addProgram(ProgramType::SKINPOSITION_BUMPEDNORMAL_TEXTURE_3D);
    addProgram(ProgramType::TERRAIN_3D);
    addProgram(ProgramType::PARTICLE_TEXTURE_3D);
    addProgram(ProgramType::PARTICLE_COLOR_3D);
    return true;
}

void ProgramCache::addProgram(ProgramType type) {
    Program* program = nullptr;
//    switch (type) {
//        case ProgramType::POSITION_TEXTURE_COLOR: {
//            program = backend::Device::getInstance()->newProgram(positionTextureColor_vert, positionTextureColor_frag);
//            break;
//        }
//        case ProgramType::ETC1:
//        {
//            program = backend::Device::getInstance()->newProgram(positionTextureColor_vert, etc1_frag);
//            break;
//        }
//        case ProgramType::LABEL_DISTANCE_NORMAL:
//        {
//            program = backend::Device::getInstance()->newProgram(positionTextureColor_vert, label_distanceNormal_frag);
//            break;
//        }
//        case ProgramType::LABEL_NORMAL:
//        {
//            program = backend::Device::getInstance()->newProgram(positionTextureColor_vert, label_normal_frag);
//            break;
//        }
//        case ProgramType::LABLE_OUTLINE:
//        {
//            program = backend::Device::getInstance()->newProgram(positionTextureColor_vert, labelOutline_frag);
//            break;
//        }
//        case ProgramType::LABLE_DISTANCEFIELD_GLOW:
//        {
//            program = backend::Device::getInstance()->newProgram(positionTextureColor_vert, labelDistanceFieldGlow_frag);
//            break;
//        }
//        case ProgramType::POSITION_COLOR_LENGTH_TEXTURE:
//        {
//            program = backend::Device::getInstance()->newProgram(positionColorLengthTexture_vert, positionColorLengthTexture_frag);
//            break;
//        }
//        case ProgramType::POSITION_COLOR_TEXTURE_AS_POINTSIZE:
//        {
//            program = backend::Device::getInstance()->newProgram(positionColorTextureAsPointsize_vert, positionColor_frag);
//            break;
//        }
//        case ProgramType::POSITION_COLOR:
//        {
//            program = backend::Device::getInstance()->newProgram(positionColor_vert, positionColor_frag);
//            break;
//        }
//        case ProgramType::POSITION:
//        {
//            program = backend::Device::getInstance()->newProgram(position_vert, positionColor_frag);
//            break;
//        }
//        case ProgramType::LAYER_RADIA_GRADIENT:
//        {
//            program = backend::Device::getInstance()->newProgram(position_vert, layer_radialGradient_frag);
//            break;
//        }
//        case ProgramType::POSITION_TEXTURE:
//        {
//            program = backend::Device::getInstance()->newProgram(positionTexture_vert, positionTexture_frag);
//            break;
//        }
//        case ProgramType::POSITION_TEXTURE_COLOR_ALPHA_TEST:
//        {
//            program = backend::Device::getInstance()->newProgram(positionTextureColor_vert, positionTextureColorAlphaTest_frag);
//            break;
//        }
//        case ProgramType::POSITION_UCOLOR:
//        {
//            program = backend::Device::getInstance()->newProgram(positionUColor_vert, positionUColor_frag);
//            break;
//        }
//        case ProgramType::ETC1_GRAY:
//        {
//            program = backend::Device::getInstance()->newProgram(positionTextureColor_vert, etc1Gray_frag);
//            break;
//        }
//        case ProgramType::GRAY_SCALE:
//        {
//            program = backend::Device::getInstance()->newProgram(positionTextureColor_vert, grayScale_frag);
//            break;
//        }
//        case ProgramType::LINE_COLOR_3D:
//        {
//            program = backend::Device::getInstance()->newProgram(lineColor3D_vert, lineColor3D_frag);
//            break;
//        }
//        case ProgramType::CAMERA_CLEAR:
//        {
//            program = backend::Device::getInstance()->newProgram(cameraClear_vert, cameraClear_frag);
//            break;
//        }
//        case ProgramType::SKYBOX_3D:
//        {
//            program = backend::Device::getInstance()->newProgram(CC3D_skybox_vert, CC3D_skybox_frag);
//            break;
//        }
//        case ProgramType::SKINPOSITION_TEXTURE_3D:
//        {
//            program = backend::Device::getInstance()->newProgram(CC3D_skinPositionTexture_vert, CC3D_colorTexture_frag);
//            break;
//        }
//        case ProgramType::SKINPOSITION_NORMAL_TEXTURE_3D:
//        {
//            
//            std::string def = getShaderMacrosForLight();
//            program = backend::Device::getInstance()->newProgram(def + CC3D_skinPositionNormalTexture_vert, def + CC3D_colorNormalTexture_frag);
//            
//            break;
//        }
//        case ProgramType::POSITION_NORMAL_TEXTURE_3D:
//        {
//            std::string def = getShaderMacrosForLight();
//            program = backend::Device::getInstance()->newProgram(def + CC3D_positionNormalTexture_vert, def + CC3D_colorNormalTexture_frag);
//            break;
//        }
//            
//        case ProgramType::POSITION_TEXTURE_3D:
//        {
//            program = backend::Device::getInstance()->newProgram(CC3D_positionTexture_vert, CC3D_colorTexture_frag);
//            break;
//        }
//        case ProgramType::POSITION_3D:
//        {
//            program = backend::Device::getInstance()->newProgram(CC3D_positionTexture_vert, CC3D_color_frag);
//            break;
//        }
//            
//        case ProgramType::POSITION_NORMAL_3D:
//        {
//            std::string def = getShaderMacrosForLight();
//            program = backend::Device::getInstance()->newProgram(def + CC3D_positionNormalTexture_vert, def + CC3D_colorNormal_frag);
//            break;
//        }
//            
//        case ProgramType::POSITION_BUMPEDNORMAL_TEXTURE_3D: {
//            std::string def = getShaderMacrosForLight();
//            std::string normalMapDef = "\n#define USE_NORMAL_MAPPING 1 \n";
//            program = backend::Device::getInstance()->newProgram(def + normalMapDef + CC3D_positionNormalTexture_vert, def + normalMapDef + CC3D_colorNormalTexture_frag);
//            break;
//        }
//            
//        case ProgramType::SKINPOSITION_BUMPEDNORMAL_TEXTURE_3D:
//        {
//            std::string def = getShaderMacrosForLight();
//            std::string normalMapDef = "\n#define USE_NORMAL_MAPPING 1 \n";
//            program = backend::Device::getInstance()->newProgram(def + normalMapDef + CC3D_skinPositionNormalTexture_vert, def + normalMapDef + CC3D_colorNormalTexture_frag);
//            break;
//        }
//            
//        case ProgramType::TERRAIN_3D:
//        {
//            program = backend::Device::getInstance()->newProgram(CC3D_terrain_vert, CC3D_terrain_frag);
//            break;
//        }
//            
//        case ProgramType::PARTICLE_TEXTURE_3D:
//        {
//            program = backend::Device::getInstance()->newProgram(CC3D_particle_vert, CC3D_particleTexture_frag);
//            break;
//        }
//        case ProgramType::PARTICLE_COLOR_3D:
//        {
//            program = backend::Device::getInstance()->newProgram(CC3D_particle_vert, CC3D_particleColor_frag);
//            break;
//        }
//        default:
//            printf("Failed: Not built-in program type. \n");
//            break;
//    }
    program->setProgramType(type);
    ProgramCache::cached_programs.emplace(type, program);
}

backend::Program* ProgramCache::getBuiltinProgram(ProgramType type) const {
    const auto& iter = ProgramCache::cached_programs.find(type);
    if (ProgramCache::cached_programs.end() != iter) {
        return iter->second;
    }
    return nullptr;
}

void ProgramCache::removeProgram(backend::Program* program) {
    if (!program) {
        return;
    }
    for (auto it = cached_programs.cbegin(); it != cached_programs.cend();) {
        if (it->second == program) {
            it->second->release();
            it = cached_programs.erase(it);
            break;
        } else {
            ++it;
        }
    }
}

void ProgramCache::removeUnusedProgram() {
    for (auto iter = cached_programs.cbegin(); iter != cached_programs.cend();) {
        auto program = iter->second;
        if (program->getReferenceCount() == 1) {
            program->release();
            iter = cached_programs.erase(iter);
        } else {
            ++iter;
        }
    }
}

void ProgramCache::removeAllPrograms() {
    for (auto& program : cached_programs) {
        program.second->release();
    }
    cached_programs.clear();
}

BACKEND_END
