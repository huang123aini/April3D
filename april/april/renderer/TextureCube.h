//
//  TextureCube.hpp
//  april3d
//
//  Created by 黄世平 on 2022/4/4.
//

#ifndef TextureCube_hpp
#define TextureCube_hpp

#include <stdio.h>
#include <string>
#include <unordered_map>

#include "renderer/Texture2D.h"
#include "renderer/backend/Texture.h"

#include "base/aprilTypes.h"

APRIL_BEGIN

/**
 *立方体纹理
*/
class  TextureCube : public Ref {
public:
    /** create cube texture from 6 textures.
       @param positive_x texture for the right side of the texture cube face.
       @param negative_x texture for the up side of the texture cube face.
       @param positive_y texture for the top side of the texture cube face
       @param negative_y texture for the bottom side of the texture cube face
       @param positive_z texture for the forward side of the texture cube face.
       @param negative_z texture for the rear side of the texture cube face.
       @return  A new texture cube inited with given parameters.
    */
    static TextureCube* create(const std::string& positive_x, const std::string& negative_x,
                               const std::string& positive_y, const std::string& negative_y,
                               const std::string& positive_z, const std::string& negative_z);

    /** Sets the min filter, mag filter, wrap s and wrap t texture parameters.
    If the texture size is NPOT (non power of 2), then in can only use GL_CLAMP_TO_EDGE in GL_TEXTURE_WRAP_{S,T}.
    */
    void setTexParameters(const Texture2D::TexParams&);

    backend::TextureBackend* getBackendTexture() { return _texture; }

    /** reload texture cube after GLESContext reconstructed.*/
    bool reloadTexture();

    TextureCube();

    virtual ~TextureCube();
protected:

    bool init(const std::string& positive_x, const std::string& negative_x,
              const std::string& positive_y, const std::string& negative_y,
              const std::string& positive_z, const std::string& negative_z);
private:
    std::vector<std::string> image_path;
    backend::TextureCubemapBackend* texture_ = nullptr;
};

APRIL_END

#endif /* TextureCube_hpp */
