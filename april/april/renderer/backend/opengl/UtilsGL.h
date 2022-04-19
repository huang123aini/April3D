//
//  UtilsGL.hpp
//  april3d
//
//  Created by 黄世平 on 2022/4/3.
//

#ifndef UtilsGL_hpp
#define UtilsGL_hpp

#include <stdio.h>

#include "base/aprilMacros.h"
#include "platform/AprilGL.h"
#include "renderer/backend/Types.h"

#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>


BACKEND_BEGIN

class UtilsGL {
public:
    /**
     * Convert attribute enum classs type to GLenum type. i.e. convert VertexFormat::FLOAT4 to GL_FLOAT.
     * @param vertexFormat Specifies the attribute data type to convert.
     * @return Attribute type.
     */
    static GLenum toGLAttributeType(VertexFormat vertexFormat);

    /**
     * Convert attribute component count to GLsizei. i.e. convert VertexFormat::FLOAT4 to 4.
     * @param vertexFormat Specifies the attribute type.
     * @return Attribute component counts.
     */
    static GLsizei getGLAttributeSize(VertexFormat vertexFormat);

    /**
     * Get opengl data type size in bytes. i.e. GL_FLOAT_VEC4 data type size is sizeof(GLfloat) * 4.
     * @param size  Specifies the data type.
     * @return Data type size in bytes.
     */
    static GLsizei getGLDataTypeSize(GLenum size);

    /**
     * Convert magnification filter to GLint. i.e. convert SamplerFilter::LINEAR to GL_LINEAR.
     * @param magFilter Specifies the magnification filter to convert.
     * @return Magnification filter.
     */
    static GLint toGLMagFilter(SamplerFilter magFilter);

    /**
     * Convert minifying filter to GLint. i.e. convert SamplerFilter::LINEAR to GL_LINEAR.
     * If mipmaps is enabled and texture width and height are not power of two, then if minFilter is SamplerFilter::LINEAR, GL_LINEAR is returned, otherwise return GL_NEAREST.
     * @param minFilter Specifies minifying filter.
     * @param hasMipmaps Specifies whether mipmap is enabled.
     * @param isPow2 Specifies if texture width and height are power of two.
     * @return Minifying filter
     */
    static GLint toGLMinFilter(SamplerFilter minFilter, bool hasMipmaps, bool isPow2);

    /**
     * Convert wrap parameter for texture coordinate to GLint. i.e. convert SamplerAddressMode::REPEAT to GL_REPEAT.
     * If texture width and height are not power of 2, then GL_CLAMP_TO_EDGE is returned.
     * @param addressMode Specifies wrapping mode.
     * @param isPow2 Specifies if texture width and height are power of two.
     * @return Wrap mode.
     */
    static GLint toGLAddressMode(SamplerAddressMode addressMode, bool isPow2);

    /**
     * Get textrue parameters from texture pixle format.
     * @param in textureFormat Specifies texture pixel format.
     * @param out internalFormat Specifies the internal format of the texture.
     * @param out format Specifies the format of the texel data.
     * @param out type Specifies the data type of the texel data.
     * @param out isCompressed Specifies if the texel data is compressed.
     */
    static void toGLTypes(PixelFormat textureFormat, GLint &internalFormat, GLuint &format, GLenum &type, bool &isCompressed);

    /**
     * Convert compare function to GLenum. i.e. convert CompareFunction::NEVER to GL_NEVER.
     * @param compareFunction Specifies the compare function to convert.
     * @return Compare function.
     */
    static GLenum toGLComareFunction(CompareFunction compareFunction);

    /**
     * Convert stencil operation to GLenum. i.e. convert StencilOperation::KEEP to GL_KEEP.
     * @param stencilOperation Specifies stencil operation.
     * @return Stencil operation.
     */
    static GLenum toGLStencilOperation(StencilOperation stencilOperation);

    /**
     * Convert blend operation to GLenum. i.e. convert BlendOperation::ADD to GL_FUNC_ADD.
     * @param blendOperation Specifies blend function to convert.
     * @return Blend operation.
     */
    static GLenum toGLBlendOperation(BlendOperation blendOperation);

    /**
     * Convert blend factor to GLenum. i.e. convert BlendFactor::ZERO to GL_ZERO.
     * @param blendFactor Specifies the blend factor to convert.
     * @return Blend factor.
     */
    static GLenum toGLBlendFactor(BlendFactor blendFactor);

    /**
     * Convert winding value to GLenum. i.e. convert Winding::CLOCK_WISE to GL_CW.
     * @param winding Specifies the winding value to convert.
     * @return Winding mode.
     */
    static GLenum toGLFrontFace(Winding winding);

    /**
     * Convert primitive type to GLenum. i.e. convert PrimitiveType::TRIANGLE to GL_TRIANGLES.
     * @param primitiveType Specifies the kind of primitives to convert.
     * @return Primitive type.
     */
    static GLenum toGLPrimitiveType(PrimitiveType primitiveType);

    /**
     * Convert index type to GLenum. i.e. convert IndexFormat::U_INT to GL_UNSIGNED_INT.
     * @param indexType Specifies the index type to convert.
     * @return Index type.
     */
    static GLenum toGLIndexType(IndexFormat indexType);

    /**
     * Convert cull mode to GLenum. i.e. convert CullMode::BACK to GL_BACK.
     * @param mode Specifies the cull mode to convert.
     * @return Cull mode.
     */
    static GLenum toGLCullMode(CullMode mode);
};

BACKEND_END


#endif /* UtilsGL_hpp */
