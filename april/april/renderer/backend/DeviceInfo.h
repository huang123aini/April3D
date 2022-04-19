//
//  DeviceInfo.h
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/2.
//

#ifndef DeviceInfo_h
#define DeviceInfo_h

#include <string>

#include "Macros.h"

BACKEND_BEGIN

enum class FeatureType : uint32_t {
    ETC1,
    S3TC,
    AMD_COMPRESSED_ATC,
    PVRTC,
    IMG_FORMAT_BGRA8888,
    DISCARD_FRAMEBUFFER,
    PACKED_DEPTH_STENCIL,
    VAO,
    MAPBUFFER,
    DEPTH24,
    ASTC
};

class DeviceInfo {
public:
    virtual ~DeviceInfo() = default;

    virtual bool init() = 0;

    virtual const char* getVendor() const = 0;

    virtual const char* getRenderer() const = 0;

    virtual const char* getVersion() const = 0;

    virtual const char* getExtension() const = 0;

    virtual bool checkForFeatureSupported(FeatureType feature) = 0;
    
    inline int getMaxTextureSize() const { return max_texture_size; }

    inline int getMaxAttributes() const { return max_attributes; }

    inline int getMaxTextureUnits() const { return max_texture_units; }

    inline int getMaxSamplesAllowed() const { return max_samples_allowed; }
    
protected:
    DeviceInfo() = default;
    int max_attributes = 0;
    int max_texture_size = 0;
    int max_texture_units = 0;
    int max_samples_allowed = 0;
};

BACKEND_END

#endif /* DeviceInfo_h */
