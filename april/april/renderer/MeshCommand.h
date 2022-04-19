//
//  MeshCommand.hpp
//  april3d
//
//  Created by 黄世平 on 2022/4/4.
//

#ifndef MeshCommand_hpp
#define MeshCommand_hpp

#include <stdio.h>

#include <unordered_map>
#include "renderer/RenderCommand.h"
#include "renderer/RenderState.h"
#include "renderer/backend/ProgramState.h"
#include "renderer/backend/Types.h"
#include "renderer/CustomCommand.h"
#include "math/Math.h"

APRIL_BEGIN

class GLProgramState;
class Material;

class MeshCommand : public CustomCommand {
public:
 
    using BufferUsage = backend::BufferUsage;
 
    using IndexFormat = backend::IndexFormat;

    MeshCommand();
    virtual ~MeshCommand();
    MeshCommand(const MeshCommand &) = default;

 
    void init(float globalZOrder);

    void init(float globalZOrder, const Mat4 &transform);
};

APRIL_END


#endif /* MeshCommand_hpp */
