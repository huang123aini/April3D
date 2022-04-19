//
//  aprilUtils.hpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/5.
//

#ifndef aprilUtils_hpp
#define aprilUtils_hpp

#include <stdio.h>
#include <vector>
#include <string>
#include "scene/2d/Node.h"
#include "base/aprilMacros.h"
#include "base/Data.h"
#include "renderer/backend/Types.h"
#include "math/Mat4.h"

APRIL_BEGIN

int aprilNextPOT(int value);

class Sprite;
class Image;

namespace utils {
void  captureScreen(const std::function<void(bool, const std::string&)>& afterCaptured, const std::string& filename);


void captureNode(Node* startNode, std::function<void(Image*)> imageCallback, float scale = 1.0f);


std::vector<Node*>  findChildren(const Node &node, const std::string &name);


double  atof(const char* str);


double  gettime();


long long  getTimeInMilliseconds();

Rect getCascadeBoundingBox(Node *node);


Sprite* createSpriteFromBase64Cached(const char* base64String, const char* key);


Sprite* createSpriteFromBase64(const char* base64String);

Node*  findChild(Node* levelRoot, const std::string& name);


Node*  findChild(Node* levelRoot, int tag);


template<typename T> inline
T findChild(Node* levelRoot, const std::string& name) {
    return dynamic_cast<T>(findChild(levelRoot, name));
}

template<typename T> inline
T findChild(Node* levelRoot, int tag) {
    return dynamic_cast<T>(findChild(levelRoot, tag));
}

std::string getFileMD5Hash(const std::string &filename);

std::string getDataMD5Hash(const Data &data);

LanguageType getLanguageTypeByISO2(const char* code);

backend::BlendFactor toBackendBlendFactor(int factor);

int toGLBlendFactor(backend::BlendFactor blendFactor);

backend::SamplerFilter toBackendSamplerFilter(int mode);

backend::SamplerAddressMode toBackendAddressMode(int mode);

const Mat4& getAdjustMatrix();

std::vector<float> getNormalMat3OfMat4(const Mat4 &mat);

std::vector<int> parseIntegerList(const std::string &intsString);

}

APRIL_END

#endif /* aprilUtils_hpp */
