//
//  DepthStencilStateGL.cpp
//  april3d
//
//  Created by 黄世平 on 2022/4/3.
//

#include "DepthStencilStateGL.h"
#include "platform/AprilGL.h"
#include "base/aprilMacros.h"
#include "UtilsGL.h"


#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>

BACKEND_BEGIN

void DepthStencilStateGL::reset() {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
}

DepthStencilStateGL::DepthStencilStateGL(const DepthStencilDescriptor& descriptor)
: DepthStencilState(descriptor)
{}

void DepthStencilStateGL::apply(unsigned int stencilReferenceValueFront, unsigned int stencilReferenceValueBack) const {
    /**
     *深度测试
     */
    if (depth_stencil_descriptor.depth_test_enabled) {
        glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }
    
    if (depth_stencil_descriptor.depth_write_enabled) {
        glDepthMask(GL_TRUE);
    } else {
        glDepthMask(GL_FALSE);
    }

    glDepthFunc(UtilsGL::toGLComareFunction(depth_stencil_descriptor.depth_compare_function));
    
    /**
     *模板测试
     */
    if (depth_stencil_descriptor.stencil_test_enabled) {
        glEnable(GL_STENCIL_TEST);
    } else {
        glDisable(GL_STENCIL_TEST);
    }
    if (depth_stencil_descriptor.stencil_test_enabled) {
        if (is_back_front_stencil_equal) {
            glStencilFunc(UtilsGL::toGLComareFunction(depth_stencil_descriptor.front_face_stencil.stencil_compare_function),
                          stencilReferenceValueFront,
                          depth_stencil_descriptor.front_face_stencil.read_mask);
            glStencilOp(UtilsGL::toGLStencilOperation(depth_stencil_descriptor.front_face_stencil.stencil_failure_operation),
                        UtilsGL::toGLStencilOperation(depth_stencil_descriptor.front_face_stencil.stencil_failure_operation),
                        UtilsGL::toGLStencilOperation(depth_stencil_descriptor.front_face_stencil.stencil_failure_operation));
            glStencilMask(depth_stencil_descriptor.front_face_stencil.write_mask);
        } else {
            glStencilFuncSeparate(GL_BACK,
                                  UtilsGL::toGLComareFunction(depth_stencil_descriptor.back_face_stencil.stencil_compare_function),
                                  stencilReferenceValueBack,
                                  depth_stencil_descriptor.back_face_stencil.read_mask);
            glStencilFuncSeparate(GL_FRONT,
                                  UtilsGL::toGLComareFunction(depth_stencil_descriptor.front_face_stencil.stencil_compare_function),
                                  stencilReferenceValueFront,
                                  depth_stencil_descriptor.front_face_stencil.read_mask);
            
            glStencilOpSeparate(GL_BACK,
                                UtilsGL::toGLStencilOperation(depth_stencil_descriptor.back_face_stencil.stencil_failure_operation),
                                UtilsGL::toGLStencilOperation(depth_stencil_descriptor.back_face_stencil.depth_failure_operation),
                                UtilsGL::toGLStencilOperation(depth_stencil_descriptor.back_face_stencil.depth_stencil_pass_operation));
            glStencilOpSeparate(GL_FRONT,
                                UtilsGL::toGLStencilOperation(depth_stencil_descriptor.front_face_stencil.stencil_failure_operation),
                                UtilsGL::toGLStencilOperation(depth_stencil_descriptor.front_face_stencil.depth_failure_operation),
                                UtilsGL::toGLStencilOperation(depth_stencil_descriptor.front_face_stencil.depth_stencil_pass_operation));
            
            glStencilMaskSeparate(GL_BACK, depth_stencil_descriptor.back_face_stencil.write_mask);
            glStencilMaskSeparate(GL_FRONT, depth_stencil_descriptor.front_face_stencil.write_mask);
        }
    }
    
    CHECK_GL_ERROR_DEBUG();
}

BACKEND_END
