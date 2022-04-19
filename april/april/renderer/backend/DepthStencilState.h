//
//  DepthStencilState.hpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/2.
//

#ifndef DepthStencilState_hpp
#define DepthStencilState_hpp

#include <stdio.h>

#include "Macros.h"
#include "Types.h"

#include "base/Ref.h"

BACKEND_BEGIN

struct StencilDescriptor {
    bool operator ==(const StencilDescriptor& rhs) const;
    
    StencilOperation stencil_failure_operation = StencilOperation::KEEP;
    StencilOperation depth_failure_operation = StencilOperation::KEEP;
    StencilOperation depth_stencil_pass_operation = StencilOperation::KEEP;
    CompareFunction stencil_compare_function = CompareFunction::ALWAYS;
    unsigned int read_mask = 0;
    unsigned int write_mask = 0;
};


struct DepthStencilDescriptor {
    CompareFunction depth_compare_function = CompareFunction::LESS;
    bool depth_write_enabled = false;
    bool depth_test_enabled = false;
    
    bool stencil_test_enabled = false;
    StencilDescriptor back_face_stencil;
    StencilDescriptor front_face_stencil;
};


class DepthStencilState : public april3d::Ref {
protected:
    DepthStencilState(const DepthStencilDescriptor& descriptor);
    virtual ~DepthStencilState();
    
    DepthStencilDescriptor depth_stencil_descriptor;
    /**
     *Does front stencil status equals to back stencil's.
     */
    bool is_back_front_stencil_equal = false;
};

BACKEND_END


#endif /* DepthStencilState_hpp */
