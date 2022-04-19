//
//  Configuration.hpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/5.
//

#ifndef Configuration_hpp
#define Configuration_hpp

#include <stdio.h>
#include <string>

#include "base/Ref.h"
#include "base/Value.h"
//#include "3d/CCAnimate3D.h"

APRIL_BEGIN

class EventCustom;

class  Configuration : public Ref {
public:
    static Configuration *getInstance();

    static void destroyInstance();
public:

    virtual ~Configuration();

    int getMaxTextureSize() const;

    int getMaxModelviewStackDepth() const;

    int getMaxTextureUnits() const;

    /** Whether or not the GPU supports NPOT (Non Power Of Two) textures.
     *  OpenGL ES 2.0 already supports NPOT (iOS).
     */
    bool supportsNPOT() const;

    bool supportsPVRTC() const;
    
    bool supportsETC() const;
    
    bool supportsS3TC() const;
    
    bool supportsATITC() const;
    
    bool supportsBGRA8888() const;

    bool supportsDiscardFramebuffer() const;

    bool supportsShareableVAO() const;

    bool supportsOESDepth24() const;
    
    bool supportsOESPackedDepthStencil() const;

    bool supportsMapBuffer() const;

    int getMaxSupportDirLightInShader() const;
    
    int getMaxSupportPointLightInShader() const;
    
    int getMaxSupportSpotLightInShader() const;

    /** get 3d animate quality*/
   // Animate3DQuality getAnimate3DQuality() const;
    
    bool checkForGLExtension(const std::string &searchName) const;

    bool init();

    const Value& getValue(const std::string& key, const Value& defaultValue = Value::Null) const;

    void setValue(const std::string& key, const Value& value);

    std::string getInfo() const;

    void gatherGPUInfo();

    void loadConfigFile(const std::string& filename);
    
    static const char* CONFIG_FILE_LOADED;
    
    int getMaxAttributes() const;

private:
    Configuration();
    static Configuration    *s_sharedConfiguration;
    static std::string        s_configfile;
    
protected:
    int             max_model_view_stack_depth;
    bool            supports_PVRTC;
    bool            supports_ETC1;
    bool            supports_S3TC;
    bool            supports_ATITC;
    bool            supports_NPOT;
    bool            supports_BGRA8888;
    bool            supports_discard_framebuffer;
    bool            supports_vertex_array_object;
    bool            supports_OES_map_buffer;
    bool            supports_OES_depth24;
    bool            supports_OES_packed_depth_stencil;
    
    std::string     gl_extensions;
    /**
     *max support directional light in shader
     */
    int             max_dir_light_in_shader;
    /**
     *max support point light in shader
     */
    int             max_point_light_in_shader;
    /**
     *max support spot light in shader
     */
    int             max_spot_light_in_shader;
    
   // Animate3DQuality  animate_3d_quality; // animate 3d quality
    
    ValueMap        value_dictionary;
    
    EventCustom*    loaded_event;
};

APRIL_END

#endif /* Configuration_hpp */
