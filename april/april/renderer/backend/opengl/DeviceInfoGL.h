//
//  DeviceInfoGL.hpp
//  april3d
//
//  Created by 黄世平 on 2022/4/3.
//

#ifndef DeviceInfoGL_hpp
#define DeviceInfoGL_hpp

#include <stdio.h>

#include "../DeviceInfo.h"

BACKEND_BEGIN

/**
 * Used to query features and implementation limits
 */
class DeviceInfoGL: public DeviceInfo
{
public:
    DeviceInfoGL() = default;
    virtual ~DeviceInfoGL() = default;

    /**
     * Gather features and implementation limits
     */
    virtual bool init() override;

    /**
     * Get vendor device name.
     * @return Vendor device name.
     */
    virtual const char* getVendor() const override;

    /**
     * Get the full name of the vendor device.
     * @return The full name of the vendor device.
     */
    virtual const char* getRenderer() const override;

    /**
     * Get version name.
     * @return Version name.
     */
    virtual const char* getVersion() const override;

    /**
     * get OpenGL ES extensions.
     * @return Extension supported by OpenGL ES.
     */
    virtual const char* getExtension() const override;

    /**
     * Check if feature supported by OpenGL ES.
     * @param feature Specify feature to be query.
     * @return true if the feature is supported, false otherwise.
     */
    virtual bool checkForFeatureSupported(FeatureType feature) override;
    
private:
    bool checkForGLExtension(const std::string &searchName) const;

    std::string gl_extensions;
};

BACKEND_END


#endif /* DeviceInfoGL_hpp */
