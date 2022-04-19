//
//  DepthStencilStateGL.hpp
//  april3d
//
//  Created by 黄世平 on 2022/4/3.
//

#ifndef DepthStencilStateGL_hpp
#define DepthStencilStateGL_hpp

#include <stdio.h>

#include "../DepthStencilState.h"

BACKEND_BEGIN

class DepthStencilStateGL : public DepthStencilState
{
public:
    static void reset();
    
    DepthStencilStateGL(const DepthStencilDescriptor& descriptor);

    void apply(unsigned int stencilReferenceValueFront, unsigned int stencilReferenceValueBack) const;
};

BACKEND_END

#endif /* DepthStencilStateGL_hpp */
