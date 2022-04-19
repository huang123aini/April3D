//
//  RenderState.hpp
//  april3d
//
//  Created by 黄世平 on 2022/4/4.
//

#ifndef RenderState_hpp
#define RenderState_hpp

#include <stdio.h>
#include <string>
#include <functional>
#include <cstdint>

#include "platform/AprilPlatformMacros.h"
#include "base/Ref.h"
#include "base/aprilTypes.h"
#include "base/Vector.h"

#include "renderer/PipelineDescriptor.h"
#include "renderer/backend/Types.h"
#include "renderer/MeshCommand.h"

APRIL_BEGIN

class Texture2D;
class Pass;
class MeshCommand;

using CullFaceSide = backend::CullMode;
using FrontFace = backend::Winding;
using DepthFunction = backend::CompareFunction;

/**
 *渲染状态
 */
class  RenderState : public Ref {
    friend class Material;
    friend class Technique;
    friend class Pass;

public:

    std::string getName() const;

    void bindPass(Pass* pass, MeshCommand*);

    class  StateBlock {
        friend class RenderState;
        friend class Pass;
        friend class RenderQueue;
        friend class Renderer;
          public:
        StateBlock() = default;
        ~StateBlock() = default;
        StateBlock(const StateBlock &) = default;
      
        void bind(PipelineDescriptor *programState);

        void setBlendFunc(const BlendFunc& blendFunc);

        void setBlend(bool enabled);

        void setBlendSrc(backend::BlendFactor blend);
    
        void setBlendDst(backend::BlendFactor blend);

        void setCullFace(bool enabled);

        void setCullFaceSide(CullFaceSide side);

        void setFrontFace(FrontFace winding);

        void setDepthTest(bool enabled);

        void setDepthWrite(bool enabled);

        void setDepthFunction(DepthFunction func);

        void setState(const std::string& name, const std::string& value);

        uint32_t getHash() const;
    
        bool isDirty() const;

        enum {
            RS_BLEND = (1 << 0),
            RS_BLEND_FUNC = (1 << 1),
            RS_CULL_FACE = (1 << 2),
            RS_DEPTH_TEST = (1 << 3),
            RS_DEPTH_WRITE = (1 << 4),
            RS_DEPTH_FUNC = (1 << 5),
            RS_CULL_FACE_SIDE = (1 << 6),
//            RS_STENCIL_TEST = (1 << 7),
//            RS_STENCIL_WRITE = (1 << 8),
//            RS_STENCIL_FUNC = (1 << 9),
//            RS_STENCIL_OP = (1 << 10),
            RS_FRONT_FACE = (1 << 11),
            
            RS_ALL_ONES = 0xFFFFFFFF,
        };

    protected:
        
        void apply(PipelineDescriptor *pipelineDescriptor);

        static void restoreUnmodifiedStates(long flags, PipelineDescriptor *pipelineDescriptor);


        bool cull_face_enabled = false;
        bool depth_test_enabled = true;
        bool depth_write_enabled = false;
        DepthFunction depth_function = DepthFunction::LESS;
        bool blend_enabled = true;
        backend::BlendFactor blend_src = backend::BlendFactor::ONE;
        backend::BlendFactor blend_dst = backend::BlendFactor::ZERO;
        CullFaceSide cull_face_side = CullFaceSide::BACK;
        FrontFace front_face = FrontFace::COUNTER_CLOCK_WISE;
        long modified_bits = 0L;

        mutable uint32_t hash_;
        mutable bool hash_dirty;
    };

    StateBlock& getStateBlock() const;

protected:
    RenderState() = default;
    
    mutable uint32_t hash_ = 0;
    mutable bool hash_dirty = true;

    mutable StateBlock state_;

    std::string name_;
};

APRIL_END

#endif /* RenderState_hpp */
