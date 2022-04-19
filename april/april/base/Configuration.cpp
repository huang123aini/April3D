//
//  Configuration.cpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/5.
//

#include "base/Configuration.h"
//#include "platform/CCFileUtils.h"
//#include "base/CCEventCustom.h"
//#include "base/CCDirector.h"
//#include "base/CCEventDispatcher.h"
#include "renderer/backend/Device.h"

APRIL_BEGIN

extern const char* april3dVersion();

Configuration* Configuration::s_sharedConfiguration = nullptr;

const char* Configuration::CONFIG_FILE_LOADED = "config_file_loaded";

Configuration::Configuration()
: max_model_view_stack_depth(0)
, supports_PVRTC(false)
, supports_ETC1(false)
, supports_S3TC(false)
, supports_ATITC(false)
, supports_NPOT(false)
, supports_BGRA8888(false)
, supports_discard_framebuffer(false)
, supports_vertex_array_object(false)
, supports_OES_map_buffer(false)
, supports_OES_depth24(false)
, supports_OES_packed_depth_stencil(false)
, max_dir_light_in_shader(1)
, max_point_light_in_shader(1)
, max_spot_light_in_shader(1)
//, animate_3d_quality(Animate3DQuality::QUALITY_LOW)
{
    //loaded_event = new (std::nothrow) EventCustom(CONFIG_FILE_LOADED);
}

bool Configuration::init() {
    value_dictionary["april3d.version"] = Value(cocos2dVersion());


#if CC_ENABLE_PROFILERS
    value_dictionary["april3d.compiled_with_profiler"] = Value(true);
#else
    value_dictionary["april3d.compiled_with_profiler"] = Value(false);
#endif

#if CC_ENABLE_GL_STATE_CACHE == 0
    value_dictionary["april3d.compiled_with_gl_state_cache"] = Value(false);
#else
    value_dictionary["april3d.compiled_with_gl_state_cache"] = Value(true);
#endif

#if APRIL_DEBUG
    value_dictionary["april3d.build_type"] = Value("DEBUG");
#else
    value_dictionary["april3d.build_type"] = Value("RELEASE");
#endif

    return true;
}

Configuration::~Configuration() {
    SAFE_DELETE(_loadedEvent);
}

std::string Configuration::getInfo() const {

#if ENABLE_PROFILERS
    printf("cocos2d: **** WARNING **** CC_ENABLE_PROFILERS is defined. Disable it when you finish profiling (from ccConfig.h)\n");
#endif

#if CC_ENABLE_GL_STATE_CACHE == 0
    CCLOG("cocos2d: **** WARNING **** CC_ENABLE_GL_STATE_CACHE is disabled. To improve performance, enable it (from ccConfig.h)\n");
#endif

    // Dump
    Value forDump = Value(value_dictionary);
    return forDump.getDescription();
}

void Configuration::gatherGPUInfo() {
    auto device_info = backend::Device::getInstance()->getDeviceInfo();
    value_dictionary["vendor"] = Value(device_info->getVendor());
    value_dictionary["renderer"] = Value(device_info->getRenderer());
    value_dictionary["version"] = Value(device_info->getVersion());
    
    value_dictionary["max_texture_size"] = Value(device_info->getMaxTextureSize());
    value_dictionary["max_vertex_attributes"] = Value(device_info->getMaxAttributes());
    value_dictionary["max_texture_units"] = Value(device_info->getMaxTextureUnits());
    value_dictionary["max_samples_allowed"] = Value(device_info->getMaxSamplesAllowed());
    
    supports_NPOT = true;
    value_dictionary["supports_NPOT"] = Value(supports_NPOT);
    
    supports_ETC1 = device_info->checkForFeatureSupported(backend::FeatureType::ETC1);
    value_dictionary["supports_ETC1"] = Value(supports_ETC1);
    
    supports_S3TC = device_info->checkForFeatureSupported(backend::FeatureType::S3TC);
    value_dictionary["supports_S3TC"] = Value(supports_S3TC);
    
    supports_ATITC = device_info->checkForFeatureSupported(backend::FeatureType::AMD_COMPRESSED_ATC);
    value_dictionary["supports_ATITC"] = Value(supports_ATITC);
    
    supports_PVRTC = device_info->checkForFeatureSupported(backend::FeatureType::PVRTC);
    value_dictionary["supports_PVRTC"] = Value(supports_PVRTC);
    
    supports_BGRA8888 = device_info->checkForFeatureSupported(backend::FeatureType::IMG_FORMAT_BGRA8888);
    value_dictionary["supports_BGRA8888"] = Value(supports_BGRA8888);
    
    supports_discard_framebuffer = device_info->checkForFeatureSupported(backend::FeatureType::DISCARD_FRAMEBUFFER);
    value_dictionary["supports_discard_framebuffer"] = Value(supports_discard_framebuffer);
    
    supports_OES_packed_depth_stencil = device_info->checkForFeatureSupported(backend::FeatureType::PACKED_DEPTH_STENCIL);
    value_dictionary["supports_OES_packed_depth_stencil"] = Value(supports_OES_packed_depth_stencil);
    
    supports_vertex_array_object = device_info->checkForFeatureSupported(backend::FeatureType::VAO);
    value_dictionary["supports_vertex_array_object"] = Value(supports_vertex_array_object);
    
    supports_OES_map_buffer = device_info->checkForFeatureSupported(backend::FeatureType::MAPBUFFER);
    value_dictionary["supports_OES_map_buffer"] = Value(supports_OES_map_buffer);
    
    supports_OES_depth24 = device_info->checkForFeatureSupported(backend::FeatureType::DEPTH24);
    value_dictionary["supports_OES_depth24"] = Value(supports_OES_depth24);
    
    _glExtensions = device_info->getExtension();
}

Configuration* Configuration::getInstance() {
    if (! s_sharedConfiguration) {
        s_sharedConfiguration = new (std::nothrow) Configuration();
        s_sharedConfiguration->init();
    }
    
    return s_sharedConfiguration;
}

void Configuration::destroyInstance() {
    SAFE_RELEASE_NULL(s_sharedConfiguration);
}


bool Configuration::checkForGLExtension(const std::string &searchName) const {
    return gl_extensions.find(searchName) != std::string::npos;
}

int Configuration::getMaxTextureSize() const {
    auto device_info = backend::Device::getInstance()->getDeviceInfo();
    return device_info->getMaxTextureSize();
}

int Configuration::getMaxModelviewStackDepth() const
{
    return _maxModelviewStackDepth;
}

int Configuration::getMaxTextureUnits() const
{
    auto device_info = backend::Device::getInstance()->getDeviceInfo();
    return device_info->getMaxTextureUnits();
}

bool Configuration::supportsNPOT() const {
    return supports_NPOT;
}

bool Configuration::supportsPVRTC() const {
    return supports_PVRTC;
}

bool Configuration::supportsETC() const
{
    return supports_ETC1;
}

bool Configuration::supportsS3TC() const
{
    return supports_S3TC;
}

bool Configuration::supportsATITC() const
{
    return _supportsATITC;
}

bool Configuration::supportsBGRA8888() const {
    return supports_BGRA8888;
}

bool Configuration::supportsDiscardFramebuffer() const {
    return supports_discard_framebuffer;
}

bool Configuration::supportsShareableVAO() const {
#if TEXTURE_ATLAS_USE_VAO
    return supports_vertex_array_object;
#else
    return false;
#endif
}

bool Configuration::supportsMapBuffer() const {
#if (APRIL_OS == APRIL_OS_ANDROID)
    return supports_OES_map_buffer;
#else
    return true;
#endif
}

bool Configuration::supportsOESDepth24() const
{
    return _supportsOESDepth24;
    
}
bool Configuration::supportsOESPackedDepthStencil() const
{
    return _supportsOESPackedDepthStencil;
}

int Configuration::getMaxSupportDirLightInShader() const
{
    return max_dir_light_in_shader;
}

int Configuration::getMaxSupportPointLightInShader() const
{
    return max_point_light_in_shader;
}

int Configuration::getMaxSupportSpotLightInShader() const
{
    return max_spot_light_in_shader;
}

/**
Animate3DQuality Configuration::getAnimate3DQuality() const {
    return _animate3DQuality;
}
*/


const Value& Configuration::getValue(const std::string& key, const Value& defaultValue) const {
    auto iter = value_dictionary.find(key);
    if (iter != value_dictionary.cend())
        return iter->second;

    return defaultValue;
}

void Configuration::setValue(const std::string& key, const Value& value) {
    value_dictionary[key] = value;
}


void Configuration::loadConfigFile(const std::string& filename) {
    ValueMap dict = FileUtils::getInstance()->getValueMapFromFile(filename);
    CCASSERT(!dict.empty(), "cannot create dictionary");

    // search for metadata
    bool validMetadata = false;
    auto metadataIter = dict.find("metadata");
    if (metadataIter != dict.cend() && metadataIter->second.getType() == Value::Type::MAP)
    {
        
        const auto& metadata = metadataIter->second.asValueMap();
        auto formatIter = metadata.find("format");
        
        if (formatIter != metadata.cend())
        {
            int format = formatIter->second.asInt();

            // Support format: 1
            if (format == 1)
            {
                validMetadata = true;
            }
        }
    }

    if (! validMetadata)
    {
        CCLOG("Invalid config format for file: %s", filename.c_str());
        return;
    }

    auto dataIter = dict.find("data");
    if (dataIter == dict.cend() || dataIter->second.getType() != Value::Type::MAP)
    {
        CCLOG("Expected 'data' dict, but not found. Config file: %s", filename.c_str());
        return;
    }

    // Add all keys in the existing dictionary
    
    const auto& dataMap = dataIter->second.asValueMap();
    for (const auto& dataMapIter : dataMap)
    {
        if (value_dictionary.find(dataMapIter.first) == value_dictionary.cend())
            value_dictionary[dataMapIter.first] = dataMapIter.second;
        else
            printf("Key already present. Ignoring '%s' \n",dataMapIter.first.c_str());
    }
    
    //light info
    std::string name = "april3d.3d.max_dir_light_in_shader";
    if (value_dictionary.find(name) != value_dictionary.end())
        max_dir_light_in_shader = value_dictionary[name].asInt();
    else
        value_dictionary[name] = Value(max_dir_light_in_shader);
    
    name = "april3d.3d.max_point_light_in_shader";
    if (value_dictionary.find(name) != value_dictionary.end()) {
        max_point_light_in_shader = value_dictionary[name].asInt();
    } else {
        value_dictionary[name] = Value(max_point_light_in_shader);
    }

    name = "april3d.3d.max_spot_light_in_shader";
    if (value_dictionary.find(name) != value_dictionary.end()) {
        max_spot_light_in_shader = value_dictionary[name].asInt();
    } else {
        value_dictionary[name] = Value(max_spot_light_in_shader);
    }

    /**
    name = "april3d.3d.animate_quality";
    if (value_dictionary.find(name) != value_dictionary.end())
        _animate3DQuality = (Animate3DQuality)value_dictionary[name].asInt();
    else
        value_dictionary[name] = Value((int)_animate3DQuality);
     */
    
    /**
    Director::getInstance()->getEventDispatcher()->dispatchEvent(_loadedEvent);
     */
}

int Configuration::getMaxAttributes() const {
    auto device_info = backend::Device::getInstance()->getDeviceInfo();
    return device_info->getMaxAttributes();
}

APRIL_END

