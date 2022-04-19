//
//  QuadCommand.hpp
//  april3d
//
//  Created by 黄世平 on 2022/4/4.
//

#ifndef QuadCommand_hpp
#define QuadCommand_hpp

#include <stdio.h>
#include <vector>

#include "renderer/TrianglesCommand.h"

APRIL_BEGIN

class QuadCommand : public TrianglesCommand {
public:
    QuadCommand();
    ~QuadCommand();
    void init(float globalOrder, Texture2D* texture, const BlendFunc& blendType, V3F_C4B_T2F_Quad* quads, ssize_t quadCount, const Mat4& mv, uint32_t flags /**is using 3d rendering or not*/);
    
protected:
    void reIndex(int indices);
    int index_size;
    std::vector<uint16_t*> owned_indices;

    // shared across all instances
    static int index_capacity;
    static uint16_t* indices_;
};

APRIL_END

#endif /* QuadCommand_hpp */
