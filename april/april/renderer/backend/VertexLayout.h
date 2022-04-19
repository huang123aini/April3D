//
//  VertexLayout.hpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/2.
//

#ifndef VertexLayout_hpp
#define VertexLayout_hpp

#include <stdio.h>
#include <string>
#include <cstdint>
#include <vector>
#include <unordered_map>

#include "Macros.h"
#include "Types.h"
#include "base/Ref.h"



BACKEND_BEGIN
/**
 *@brief 顶点属性布局
 */
class VertexLayout {
public:
    struct Attribute
    {
        Attribute() = default;
        Attribute(const std::string& name, std::size_t index, VertexFormat format, std::size_t offset, bool needToBeNormallized)
        : name_(name)
        , format_(format)
        , offset_(offset)
        , index_(index)
        , need_to_be_normallized(needToBeNormallized)
        {}
        
        /**
         *name is used in opengl
         */
        std::string name_;
        VertexFormat format_ = VertexFormat::INT3;
        std::size_t offset_ = 0;
        /**
         *index is used in metal
         */
        std::size_t index_ = 0;
        bool need_to_be_normallized = false;
    };

    VertexLayout() = default;
    
    /**
     * Set attribute values to name.
     */
    void setAttribute(const std::string& name, std::size_t index, VertexFormat format, std::size_t offset, bool needToBeNormallized);
    
    /**
     * Set stride of vertices.
     */
    void setLayout(std::size_t stride);
    
    inline std::size_t getStride() const { return stride_; }

    inline VertexStepMode getVertexStepMode() const { return step_mode; }

    inline const std::unordered_map<std::string, Attribute>& getAttributes() const { return attributes_; }

    inline bool isValid() const { return stride_ != 0; }
    
private:
    std::unordered_map<std::string, Attribute> attributes_;
    std::size_t stride_ = 0;
    VertexStepMode step_mode = VertexStepMode::VERTEX;
};

BACKEND_END

#endif /* VertexLayout_hpp */
