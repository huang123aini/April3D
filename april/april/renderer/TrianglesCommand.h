//
//  TrianglesCommand.hpp
//  april3d
//
//  Created by 黄世平 on 2022/4/4.
//

#ifndef TrianglesCommand_hpp
#define TrianglesCommand_hpp

#include <stdio.h>
#include "renderer/RenderCommand.h"
#include "renderer/PipelineDescriptor.h"

APRIL_BEGIN

namespace backend {
    class TextureBackend;
    class Program;
}

class Texture2D;

class TrianglesCommand : public RenderCommand {
public:
    struct Triangles {
        Triangles(V3F_C4B_T2F* verts, unsigned short* indices, unsigned int vertCount, unsigned int indexCount)
        : verts_(verts)
        , indices_(indices)
        , vert_count(vertCount)
        , index_count(indexCount) {}

        Triangles() {}

        V3F_C4B_T2F* verts_ = nullptr;
        
        unsigned short* indices_ = nullptr;

        unsigned int vert_count = 0;

        unsigned int index_count = 0;
    };

    TrianglesCommand();
    ~TrianglesCommand();
    
    /**
     *command初始化、 globalZOrder、texture、blendType、triangles、
     */
    void init(float globalOrder, april3d::Texture2D* texture, const BlendFunc& blendType,  const Triangles& triangles, const Mat4& mv, uint32_t flags);
    uint32_t getMaterialID() const { return material_id; }

    const Triangles& getTriangles() const { return triangles_; }

    size_t getVertexCount() const { return triangles_.vert_count; }

    size_t getIndexCount() const { return triangles_.index_count; }

    const V3F_C4B_T2F* getVertices() const { return triangles_.verts_; }

    const unsigned short* getIndices() const { return triangles_.indices_; }

    const Mat4& getModelView() const { return mv_matrix; }
    
    void updateMaterialID();
  
protected:

    void generateMaterialID();

    uint32_t material_id = 0;
    
    Triangles triangles_;

    Mat4 mv_matrix;

    uint8_t alpha_texture_id = 0;

    BlendFunc blend_type = BlendFunc::DISABLE;

    backend::ProgramType program_type = backend::ProgramType::CUSTOM_PROGRAM;
    
    backend::TextureBackend* texture_ = nullptr;
};

APRIL_END

#endif /* TrianglesCommand_hpp */
