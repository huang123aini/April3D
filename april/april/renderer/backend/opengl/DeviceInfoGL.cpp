//
//  DeviceInfoGL.cpp
//  april3d
//
//  Created by 黄世平 on 2022/4/3.
//

#include "DeviceInfoGL.h"
#include "platform/AprilGL.h"

#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>

BACKEND_BEGIN

bool DeviceInfoGL::init() {
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max_attributes);
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_texture_size);
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &max_texture_units);
    gl_extensions = (const char*)glGetString(GL_EXTENSIONS);
    return true;
}

const char* DeviceInfoGL::getVendor() const {
    return (const char*)glGetString(GL_VENDOR);
}

const char* DeviceInfoGL::getRenderer() const {
    return (const char*)glGetString(GL_RENDERER);
}

const char* DeviceInfoGL::getVersion() const {
    return (const char*)glGetString(GL_VERSION);
}

const char* DeviceInfoGL::getExtension() const {
    return gl_extensions.c_str();
}

bool DeviceInfoGL::checkForFeatureSupported(FeatureType feature) {
    bool featureSupported = false;
    switch (feature) {
    case FeatureType::ETC1:
#ifdef GL_ETC1_RGB8_OES //GL_ETC1_RGB8_OES is not defined in old opengl version
        featureSupported = checkForGLExtension("GL_OES_compressed_ETC1_RGB8_texture");
#endif
        break;
    case FeatureType::S3TC:
#ifdef GL_EXT_texture_compression_s3tc
        featureSupported = checkForGLExtension("GL_EXT_texture_compression_s3tc");
#endif
        break;
    case FeatureType::AMD_COMPRESSED_ATC:
        featureSupported = checkForGLExtension("GL_AMD_compressed_ATC_texture");
        break;
    case FeatureType::PVRTC:
        featureSupported = checkForGLExtension("GL_IMG_texture_compression_pvrtc");
        break;
    case FeatureType::IMG_FORMAT_BGRA8888:
        featureSupported = checkForGLExtension("GL_IMG_texture_format_BGRA8888");
        break;
    case FeatureType::DISCARD_FRAMEBUFFER:
        featureSupported = checkForGLExtension("GL_EXT_discard_framebuffer");
        break;
    case FeatureType::PACKED_DEPTH_STENCIL:
        featureSupported = checkForGLExtension("GL_OES_packed_depth_stencil");
        break;
    case FeatureType::VAO:
#ifdef APRIL_OS_PC
        featureSupported = checkForGLExtension("vertex_array_object");
#else
        featureSupported = checkForGLExtension("GL_OES_vertex_array_object");
#endif
        break;
    case FeatureType::MAPBUFFER:
        featureSupported = checkForGLExtension("GL_OES_mapbuffer");
        break;
    case FeatureType::DEPTH24:
        featureSupported = checkForGLExtension("GL_OES_depth24");
        break;
    default:
        break;
    }
    return featureSupported;
}

bool DeviceInfoGL::checkForGLExtension(const std::string &searchName) const {
    return gl_extensions.find(searchName) != std::string::npos;
}

BACKEND_END
