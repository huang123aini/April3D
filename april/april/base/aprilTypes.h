//
//  aprilTypes.hpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/4.
//

#ifndef aprilTypes_hpp
#define aprilTypes_hpp

#include <stdio.h>
#include <string>

#include "math/Geometry.h"
#include "math/Math.h"
#include "base/Ref.h"
#include "base/EnumClass.h"
#include "renderer/backend/Types.h"
#include "renderer/backend/Macros.h"

APRIL_BEGIN

struct Color4B;
struct Color4F;

struct  Color3B {
    Color3B();
    Color3B(uint8_t _r, uint8_t _g, uint8_t _b);
    explicit Color3B(const Color4B& color);
    explicit Color3B(const Color4F& color);

    bool operator==(const Color3B& right) const;
    bool operator==(const Color4B& right) const;
    bool operator==(const Color4F& right) const;
    bool operator!=(const Color3B& right) const;
    bool operator!=(const Color4B& right) const;
    bool operator!=(const Color4F& right) const;

    bool equals(const Color3B& other) const {
        return (*this == other);
    }

    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;

    static const Color3B WHITE;
    static const Color3B YELLOW;
    static const Color3B BLUE;
    static const Color3B GREEN;
    static const Color3B RED;
    static const Color3B MAGENTA;
    static const Color3B BLACK;
    static const Color3B ORANGE;
    static const Color3B GRAY;
};

struct Color4B {
    Color4B();
    Color4B(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a);
    explicit Color4B(const Color3B& color, uint8_t _a = 255);
    explicit Color4B(const Color4F& color);
    
    inline void set(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a) {
        r = _r;
        g = _g;
        b = _b;
        a = _a;
    }

    bool operator==(const Color4B& right) const;
    bool operator==(const Color3B& right) const;
    bool operator==(const Color4F& right) const;
    bool operator!=(const Color4B& right) const;
    bool operator!=(const Color3B& right) const;
    bool operator!=(const Color4F& right) const;

    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 0;

    static const Color4B WHITE;
    static const Color4B YELLOW;
    static const Color4B BLUE;
    static const Color4B GREEN;
    static const Color4B RED;
    static const Color4B MAGENTA;
    static const Color4B BLACK;
    static const Color4B ORANGE;
    static const Color4B GRAY;
};

struct Color4F {
    Color4F();
    Color4F(float _r, float _g, float _b, float _a);
    explicit Color4F(const Color3B& color, float _a = 1.0f);
    explicit Color4F(const Color4B& color);

    bool operator==(const Color4F& right) const;
    bool operator==(const Color3B& right) const;
    bool operator==(const Color4B& right) const;
    bool operator!=(const Color4F& right) const;
    bool operator!=(const Color3B& right) const;
    bool operator!=(const Color4B& right) const;

    bool equals(const Color4F &other) const {
        return (*this == other);
    }

    float r = 0.f;
    float g = 0.f;
    float b = 0.f;
    float a = 0.f;

    static const Color4F WHITE;
    static const Color4F YELLOW;
    static const Color4F BLUE;
    static const Color4F GREEN;
    static const Color4F RED;
    static const Color4F MAGENTA;
    static const Color4F BLACK;
    static const Color4F ORANGE;
    static const Color4F GRAY;
};

Color4F& operator+=(Color4F& lhs, const Color4F& rhs);
Color4F operator+(Color4F lhs, const Color4F& rhs);

Color4F& operator-=(Color4F& lhs, const Color4F& rhs);
Color4F operator-(Color4F lhs, const Color4F& rhs);

Color4F& operator*=(Color4F& lhs, const Color4F& rhs);
Color4F operator*(Color4F lhs, const Color4F& rhs);
Color4F& operator*=(Color4F& lhs, float rhs);
Color4F operator*(Color4F lhs, float rhs);

Color4F& operator/=(Color4F& lhs, const Color4F& rhs);
Color4F operator/(Color4F lhs, const Color4F& rhs);
Color4F& operator/=(Color4F& lhs, float rhs);
Color4F operator/(Color4F lhs, float rhs);


struct Tex2F {
    Tex2F(float _u, float _v): u(_u), v(_v) {}

    Tex2F() {}

    float u = 0.f;
    float v = 0.f;
};

struct PointSprite {
    Vec2       pos;
    Color4B    color;
    float      size = 0.f;
};

/**
 *2d Quad
 */
struct Quad2 {
    Vec2        tl;
    Vec2        tr;
    Vec2        bl;
    Vec2        br;
};

/**
 *3d quad
 */
struct Quad3 {
    Vec3        bl;
    Vec3        br;
    Vec3        tl;
    Vec3        tr;
};


struct V2F_C4B_T2F {
    Vec2       vertices;
    Color4B        colors;
    Tex2F          texCoords;
};


struct V2F_C4B_PF {
    Vec2       vertices;
    Color4B        colors;
    float      pointSize = 0.f;
};

struct V2F_C4F_T2F {
    Vec2       vertices;
    Color4F        colors;
    Tex2F          texCoords;
};

struct V3F_C4B_T2F {
    Vec3     vertices;
    Color4B  colors;
    Tex2F    texCoords;
};

struct V3F_T2F {
    Vec3       vertices;
    Tex2F      texCoords;
};

struct V3F_C4F {
    Vec3 vertices;
    Color4F colors;
};

struct V2F_C4B_T2F_Triangle {
    V2F_C4B_T2F a;
    V2F_C4B_T2F b;
    V2F_C4B_T2F c;
};

struct V2F_C4B_T2F_Quad {
    V2F_C4B_T2F    bl;
    V2F_C4B_T2F    br;
    V2F_C4B_T2F    tl;
    V2F_C4B_T2F    tr;
};

struct  V3F_C4B_T2F_Quad {
    V3F_C4B_T2F    tl;
    V3F_C4B_T2F    bl;
    V3F_C4B_T2F    tr;
    V3F_C4B_T2F    br;
};

struct  V2F_C4F_T2F_Quad {
    V2F_C4F_T2F    bl;
    V2F_C4F_T2F    br;
    V2F_C4F_T2F    tl;
    V2F_C4F_T2F    tr;
};

struct  V3F_T2F_Quad {
    V3F_T2F    bl;
    V3F_T2F    br;
    V3F_T2F    tl;
    V3F_T2F    tr;
};

namespace backend {
    enum class BlendFactor: uint32_t;
}

struct  BlendFunc {
    backend::BlendFactor src;
    backend::BlendFactor dst;

    static const BlendFunc DISABLE;
    static const BlendFunc ALPHA_PREMULTIPLIED;
    static const BlendFunc ALPHA_NON_PREMULTIPLIED;
    static const BlendFunc ADDITIVE;

    bool operator==(const BlendFunc &a) const {
        return src == a.src && dst == a.dst;
    }

    bool operator!=(const BlendFunc &a) const {
        return src != a.src || dst != a.dst;
    }

    bool operator<(const BlendFunc &a) const {
        return src < a.src || (src == a.src && dst < a.dst);
    }
};

enum class TextVAlignment {
    TOP,
    CENTER,
    BOTTOM
};

enum class TextHAlignment {
    LEFT,
    CENTER,
    RIGHT
};

enum class GlyphCollection {
    DYNAMIC,
    NEHE,
    ASCII,
    CUSTOM
};

struct T2F_Quad {
    Tex2F    bl;
    Tex2F    br;
    Tex2F    tl;
    Tex2F    tr;
};

struct AnimationFrameData {
    T2F_Quad texCoords;
    Size size;
    float delay = 0.f;
};

struct FontShadow {
    Size   shadow_offset;
    float  shadow_blur = 0.f;
    float  shadow_opacity = 0.f;
    bool   shadow_enabled = false;
};

struct FontStroke {
    Color3B   stroke_color = Color3B::BLACK;
    float     stroke_size = 0.f;
    bool      stroke_enabled = false;
    uint8_t   stroke_alpha = 255;
};

struct FontDefinition {
    std::string           font_name;
    int                   font_size = 0;
    TextHAlignment        alignment_ = TextHAlignment::CENTER;
    TextVAlignment _vertAlignment = TextVAlignment::TOP;
    Size                  _dimensions = Size::ZERO;
    Color3B               _fontFillColor = Color3B::WHITE;
    uint8_t               _fontAlpha = 255;
    FontShadow            _shadow;
    FontStroke            _stroke;
    bool                  _enableWrap = true;
    int                  _overflow = 0;
};

enum class LabelEffect {
    // FIXME: Covert them to bitwise. More than one effect should be supported
    NORMAL,
    OUTLINE,
    SHADOW,
    GLOW,
    ITALICS,
    BOLD,
    UNDERLINE,
    STRIKETHROUGH,
    ALL
};

class Acceleration: public Ref {
public:
    double x = 0;
    double y = 0;
    double z = 0;

    double timestamp = 0;
};

extern const std::string  STD_STRING_EMPTY;
extern const ssize_t INVALID_INDEX;

enum class SetIntervalReason : char {
    BY_GAME = 0,
    BY_ENGINE,
    BY_SYSTEM,
    BY_SCENE_CHANGE,
    BY_DIRECTOR_PAUSE
};

struct Viewport {
    int x = 0;
    int y = 0;
    unsigned int w = 0;
    unsigned int h = 0;
};
    
struct ScissorRect {
    float x = 0;
    float y = 0;
    float width = 0;
    float height = 0;
};

enum class ClearFlag : uint8_t {
    NONE = 0,
    COLOR = 1,
    DEPTH = 1 << 1,
    STENCIL = 1 << 2,
    ALL = COLOR | DEPTH | STENCIL
};
ENABLE_BITMASK_OPERATORS(ClearFlag)

enum class RenderTargetFlag : uint8_t {
    COLOR = 1,
    DEPTH = 1 << 1,
    STENCIL = 1 << 2,
    ALL = COLOR | DEPTH | STENCIL
};
ENABLE_BITMASK_OPERATORS(RenderTargetFlag)

APRIL_END

using TextureUsage = backend::TextureUsage;
using PixelFormat = backend::PixelFormat;

#endif /* aprilTypes_hpp */
