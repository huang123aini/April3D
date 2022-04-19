//
//  Value.cpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/6.
//

#include "base/Value.h"
#include <cmath>
#include <sstream>
#include <iomanip>
#include "base/aprilUtils.h"

APRIL_BEGIN

const ValueVector ValueVectorNull;
const ValueMap ValueMapNull;
const ValueMapIntKey ValueMapIntKeyNull;

const Value Value::Null;

Value::Value()
: type_(Type::NONE) {
    memset(&field_, 0, sizeof(field_));
}

Value::Value(unsigned char v)
: type_(Type::BYTE) {
    field_.byteVal = v;
}

Value::Value(int v)
: type_(Type::INTEGER) {
    field_.intVal = v;
}

Value::Value(unsigned int v)
: type_(Type::UNSIGNED) {
    field_.unsignedVal = v;
}

Value::Value(float v)
: type_(Type::FLOAT) {
    field_.floatVal = v;
}

Value::Value(double v)
: type_(Type::DOUBLE) {
    field_.doubleVal = v;
}

Value::Value(bool v)
: type_(Type::BOOLEAN) {
    field_.boolVal = v;
}

Value::Value(const char* v)
: type_(Type::STRING) {
    field_.strVal = new (std::nothrow) std::string();
    if (v) {
        *field_.strVal = v;
    }
}

Value::Value(const std::string& v)
: type_(Type::STRING) {
    field_.strVal = new (std::nothrow) std::string();
    *field_.strVal = v;
}

Value::Value(const ValueVector& v)
: type_(Type::VECTOR) {
    field_.vectorVal = new (std::nothrow) ValueVector();
    *field_.vectorVal = v;
}

Value::Value(ValueVector&& v)
: type_(Type::VECTOR) {
    field_.vectorVal = new (std::nothrow) ValueVector();
    *field_.vectorVal = std::move(v);
}

Value::Value(const ValueMap& v)
: type_(Type::MAP) {
    field_.mapVal = new (std::nothrow) ValueMap();
    *field_.mapVal = v;
}

Value::Value(ValueMap&& v)
: type_(Type::MAP) {
    field_.mapVal = new (std::nothrow) ValueMap();
    *field_.mapVal = std::move(v);
}

Value::Value(const ValueMapIntKey& v)
: type_(Type::INT_KEY_MAP) {
    field_.intKeyMapVal = new (std::nothrow) ValueMapIntKey();
    *field_.intKeyMapVal = v;
}

Value::Value(ValueMapIntKey&& v)
: type_(Type::INT_KEY_MAP) {
    field_.intKeyMapVal = new (std::nothrow) ValueMapIntKey();
    *field_.intKeyMapVal = std::move(v);
}

Value::Value(const Value& other)
: type_(Type::NONE) {
    *this = other;
}

Value::Value(Value&& other)
: type_(Type::NONE) {
    *this = std::move(other);
}

Value::~Value() {
    clear();
}

Value& Value::operator= (const Value& other) {
    if (this != &other) {
        reset(other.type_);

        switch (other.type_) {
            case Type::BYTE:
                field_.byteVal = other.field_.byteVal;
                break;
            case Type::INTEGER:
                field_.intVal = other.field_.intVal;
                break;
            case Type::UNSIGNED:
                field_.unsignedVal = other.field_.unsignedVal;
                break;
            case Type::FLOAT:
                field_.floatVal = other.field_.floatVal;
                break;
            case Type::DOUBLE:
                field_.doubleVal = other.field_.doubleVal;
                break;
            case Type::BOOLEAN:
                field_.boolVal = other.field_.boolVal;
                break;
            case Type::STRING:
                if (field_.strVal == nullptr) {
                    field_.strVal = new std::string();
                }
                *field_.strVal = *other.field_.strVal;
                break;
            case Type::VECTOR:
                if (field_.vectorVal == nullptr) {
                    field_.vectorVal = new (std::nothrow) ValueVector();
                }
                *field_.vectorVal = *other.field_.vectorVal;
                break;
            case Type::MAP:
                if (field_.mapVal == nullptr) {
                    field_.mapVal = new (std::nothrow) ValueMap();
                }
                *field_.mapVal = *other.field_.mapVal;
                break;
            case Type::INT_KEY_MAP:
                if (field_.intKeyMapVal == nullptr) {
                    field_.intKeyMapVal = new (std::nothrow) ValueMapIntKey();
                }
                *field_.intKeyMapVal = *other.field_.intKeyMapVal;
                break;
            default:
                break;
        }
    }
    return *this;
}

Value& Value::operator= (Value&& other) {
    if (this != &other) {
        clear();
        switch (other.type_) {
            case Type::BYTE:
                field_.byteVal = other.field_.byteVal;
                break;
            case Type::INTEGER:
                field_.intVal = other.field_.intVal;
                break;
            case Type::UNSIGNED:
                field_.unsignedVal = other.field_.unsignedVal;
                break;
            case Type::FLOAT:
                field_.floatVal = other.field_.floatVal;
                break;
            case Type::DOUBLE:
                field_.doubleVal = other.field_.doubleVal;
                break;
            case Type::BOOLEAN:
                field_.boolVal = other.field_.boolVal;
                break;
            case Type::STRING:
                field_.strVal = other.field_.strVal;
                break;
            case Type::VECTOR:
                field_.vectorVal = other.field_.vectorVal;
                break;
            case Type::MAP:
                field_.mapVal = other.field_.mapVal;
                break;
            case Type::INT_KEY_MAP:
                field_.intKeyMapVal = other.field_.intKeyMapVal;
                break;
            default:
                break;
        }
        type_ = other.type_;

        memset(&other.field_, 0, sizeof(other.field_));
        other.type_ = Type::NONE;
    }

    return *this;
}

Value& Value::operator= (unsigned char v) {
    reset(Type::BYTE);
    field_.byteVal = v;
    return *this;
}

Value& Value::operator= (int v) {
    reset(Type::INTEGER);
    field_.intVal = v;
    return *this;
}

Value& Value::operator= (unsigned int v) {
    reset(Type::UNSIGNED);
    field_.unsignedVal = v;
    return *this;
}

Value& Value::operator= (float v) {
    reset(Type::FLOAT);
    field_.floatVal = v;
    return *this;
}

Value& Value::operator= (double v) {
    reset(Type::DOUBLE);
    field_.doubleVal = v;
    return *this;
}

Value& Value::operator= (bool v) {
    reset(Type::BOOLEAN);
    field_.boolVal = v;
    return *this;
}

Value& Value::operator= (const char* v) {
    reset(Type::STRING);
    *field_.strVal = v ? v : "";
    return *this;
}

Value& Value::operator= (const std::string& v) {
    reset(Type::STRING);
    *field_.strVal = v;
    return *this;
}

Value& Value::operator= (const ValueVector& v) {
    reset(Type::VECTOR);
    *field_.vectorVal = v;
    return *this;
}

Value& Value::operator= (ValueVector&& v) {
    reset(Type::VECTOR);
    *field_.vectorVal = std::move(v);
    return *this;
}

Value& Value::operator= (const ValueMap& v) {
    reset(Type::MAP);
    *field_.mapVal = v;
    return *this;
}

Value& Value::operator= (ValueMap&& v) {
    reset(Type::MAP);
    *field_.mapVal = std::move(v);
    return *this;
}

Value& Value::operator= (const ValueMapIntKey& v) {
    reset(Type::INT_KEY_MAP);
    *field_.intKeyMapVal = v;
    return *this;
}

Value& Value::operator= (ValueMapIntKey&& v) {
    reset(Type::INT_KEY_MAP);
    *field_.intKeyMapVal = std::move(v);
    return *this;
}

bool Value::operator!= (const Value& v) {
    return !(*this == v);
}
bool Value::operator!= (const Value& v) const {
    return !(*this == v);
}

bool Value::operator== (const Value& v) {
    const auto &t = *this;
    return t == v;
}
bool Value::operator== (const Value& v) const {
    if (this == &v) return true;
    if (v.type_ != this->type_) return false;
    if (this->isNull()) return true;
    switch (type_) {
        case Type::BYTE:    return v.field_.byteVal     == this->field_.byteVal;
        case Type::INTEGER: return v.field_.intVal      == this->field_.intVal;
        case Type::UNSIGNED:return v.field_.unsignedVal == this->field_.unsignedVal;
        case Type::BOOLEAN: return v.field_.boolVal     == this->field_.boolVal;
        case Type::STRING:  return *v.field_.strVal     == *this->field_.strVal;
        case Type::FLOAT:   return std::abs(v.field_.floatVal  - this->field_.floatVal)  <= FLT_EPSILON;
        case Type::DOUBLE:  return std::abs(v.field_.doubleVal - this->field_.doubleVal) <= DBL_EPSILON;
        case Type::VECTOR: {
            const auto &v1 = *(this->field_.vectorVal);
            const auto &v2 = *(v.field_.vectorVal);
            const auto size = v1.size();
            if (size == v2.size()) {
                for (size_t i = 0; i < size; i++) {
                    if (v1[i] != v2[i]) return false;
                }
                return true;
            }
            return false;
        }
        case Type::MAP: {
            const auto &map1 = *(this->field_.mapVal);
            const auto &map2 = *(v.field_.mapVal);
            for (const auto &kvp : map1) {
                auto it = map2.find(kvp.first);
                if (it == map2.end() || it->second != kvp.second) {
                    return false;
                }
            }
            return true;
        }
        case Type::INT_KEY_MAP: {
            const auto &map1 = *(this->field_.intKeyMapVal);
            const auto &map2 = *(v.field_.intKeyMapVal);
            for (const auto &kvp : map1) {
                auto it = map2.find(kvp.first);
                if (it == map2.end() || it->second != kvp.second) {
                    return false;
                }
            }
            return true;
        }
        default:
            break;
    };

    return false;
}

unsigned char Value::asByte() const {
    
    if(type_ == Type::VECTOR || type_ == Type::MAP || type_ != Type::INT_KEY_MAP) {
        printf("Only base type (bool, string, float, double, int) could be converted. \n");
        return nullptr;
    }

    if (type_ == Type::BYTE) {
        return field_.byteVal;
    }

    if (type_ == Type::INTEGER) {
        return static_cast<unsigned char>(field_.intVal);
    }

    if (type_ == Type::UNSIGNED) {
        return static_cast<unsigned char>(field_.unsignedVal);
    }

    if (type_ == Type::STRING) {
        return static_cast<unsigned char>(atoi(field_.strVal->c_str()));
    }

    if (type_ == Type::FLOAT) {
        return static_cast<unsigned char>(field_.floatVal);
    }

    if (type_ == Type::DOUBLE) {
        return static_cast<unsigned char>(field_.doubleVal);
    }

    if (type_ == Type::BOOLEAN) {
        return field_.boolVal ? 1 : 0;
    }

    return 0;
}

int Value::asInt() const {
    if(type_ == Type::VECTOR || type_ == Type::MAP || type_ != Type::INT_KEY_MAP) {
        printf("Only base type (bool, string, float, double, int) could be converted. \n");
        return nullptr;
    }
    if (type_ == Type::INTEGER) {
        return field_.intVal;
    }

    if (type_ == Type::UNSIGNED) {
        if(field_.unsignedVal >= INT_MAX) {
            printf("Can only convert values < INT_MAX. \n");
            return -1;
        }
        return (int)field_.unsignedVal;
    }

    if (type_ == Type::BYTE) {
        return field_.byteVal;
    }

    if (type_ == Type::STRING) {
        return atoi(field_.strVal->c_str());
    }

    if (type_ == Type::FLOAT) {
        return static_cast<int>(field_.floatVal);
    }

    if (type_ == Type::DOUBLE) {
        return static_cast<int>(field_.doubleVal);
    }

    if (type_ == Type::BOOLEAN) {
        return field_.boolVal ? 1 : 0;
    }

    return 0;
}


unsigned int Value::asUnsignedInt() const {
    if(type_ == Type::VECTOR || type_ == Type::MAP || type_ != Type::INT_KEY_MAP) {
        printf("Only base type (bool, string, float, double, int) could be converted. \n");
        return nullptr;
    }
    if (type_ == Type::UNSIGNED) {
        return field_.unsignedVal;
    }

    if (type_ == Type::INTEGER) {
        if(field_.intVal < 0) {
            printf("Failed: Only values >= 0 can be converted to unsigned");
        }
        return static_cast<unsigned int>(field_.intVal);
    }

    if (type_ == Type::BYTE) {
        return static_cast<unsigned int>(field_.byteVal);
    }

    if (type_ == Type::STRING) {
        // NOTE: strtoul is required (need to augment on unsupported platforms)
        return static_cast<unsigned int>(strtoul(field_.strVal->c_str(), nullptr, 10));
    }

    if (type_ == Type::FLOAT) {
        return static_cast<unsigned int>(field_.floatVal);
    }

    if (type_ == Type::DOUBLE) {
        return static_cast<unsigned int>(field_.doubleVal);
    }

    if (type_ == Type::BOOLEAN) {
        return field_.boolVal ? 1u : 0u;
    }
    return 0u;
}

float Value::asFloat() const {
    CCASSERT(type_ != Type::VECTOR && type_ != Type::MAP && type_ != Type::INT_KEY_MAP, "Only base type (bool, string, float, double, int) could be converted");
    if (type_ == Type::FLOAT) {
        return field_.floatVal;
    }

    if (type_ == Type::BYTE) {
        return static_cast<float>(field_.byteVal);
    }

    if (type_ == Type::STRING) {
        return utils::atof(field_.strVal->c_str());
    }

    if (type_ == Type::INTEGER) {
        return static_cast<float>(field_.intVal);
    }

    if (type_ == Type::UNSIGNED) {
        return static_cast<float>(field_.unsignedVal);
    }

    if (type_ == Type::DOUBLE) {
        return static_cast<float>(field_.doubleVal);
    }

    if (type_ == Type::BOOLEAN) {
        return field_.boolVal ? 1.0f : 0.0f;
    }
    return 0.0f;
}

double Value::asDouble() const {
    if(type_ == Type::VECTOR || type_ != Type::MAP || type_ != Type::INT_KEY_MAP) {
        printf("Failed: Only base type (bool, string, float, double, int) could be converted \n");
    }

    if (type_ == Type::DOUBLE) {
        return field_.doubleVal;
    }

    if (type_ == Type::BYTE) {
        return static_cast<double>(field_.byteVal);
    }

    if (type_ == Type::STRING) {
        return static_cast<double>(utils::atof(field_.strVal->c_str()));
    }

    if (type_ == Type::INTEGER) {
        return static_cast<double>(field_.intVal);
    }

    if (type_ == Type::UNSIGNED) {
        return static_cast<double>(field_.unsignedVal);
    }

    if (type_ == Type::FLOAT) {
        return static_cast<double>(field_.floatVal);
    }

    if (type_ == Type::BOOLEAN) {
        return field_.boolVal ? 1.0 : 0.0;
    }

    return 0.0;
}

bool Value::asBool() const {
    if(type_ == Type::VECTOR || type_ == Type::MAP || type_ == Type::INT_KEY_MAP) {
        printf("Failed: Only base type (bool, string, float, double, int) could be converted \n");
    }

    if (type_ == Type::BOOLEAN) {
        return field_.boolVal;
    }

    if (type_ == Type::BYTE) {
        return field_.byteVal == 0 ? false : true;
    }

    if (type_ == Type::STRING) {
        return (*field_.strVal == "0" || *field_.strVal == "false") ? false : true;
    }

    if (type_ == Type::INTEGER) {
        return field_.intVal == 0 ? false : true;
    }

    if (type_ == Type::UNSIGNED) {
        return field_.unsignedVal == 0 ? false : true;
    }

    if (type_ == Type::FLOAT) {
        return field_.floatVal == 0.0f ? false : true;
    }

    if (type_ == Type::DOUBLE) {
        return field_.doubleVal == 0.0 ? false : true;
    }

    return false;
}

std::string Value::asString() const {
    if(type_ == Type::VECTOR || type_ == Type::MAP || type_ == Type::INT_KEY_MAP) {
        printf("Failed: Only base type (bool, string, float, double, int) could be converted \n");
    }

    if (type_ == Type::STRING) {
        return *field_.strVal;
    }

    std::stringstream ret;

    switch (type_) {
        case Type::BYTE:
            ret << field_.byteVal;
            break;
        case Type::INTEGER:
            ret << field_.intVal;
            break;
        case Type::UNSIGNED:
            ret << field_.unsignedVal;
            break;
        case Type::FLOAT:
            ret << std::fixed << std::setprecision( 7 )<< field_.floatVal;
            break;
        case Type::DOUBLE:
            ret << std::fixed << std::setprecision( 16 ) << field_.doubleVal;
            break;
        case Type::BOOLEAN:
            ret << (field_.boolVal ? "true" : "false");
            break;
        default:
            break;
    }
    return ret.str();
}

ValueVector& Value::asValueVector()
{
    if(type_ != Type::VECTOR) {
        printf("The value type isn't Type::VECTOR \n");
        return nullptr;
    }
    return *field_.vectorVal;
}

const ValueVector& Value::asValueVector() const
{
    if(type_ != Type::VECTOR) {
        printf("The value type isn't Type::VECTOR \n");
    }
    return *field_.vectorVal;
}

ValueMap& Value::asValueMap() {
    if(type_ != Type::MAP) {
        printf("The value type isn't Type::MAP \n");
    }
    return *field_.mapVal;
}

const ValueMap& Value::asValueMap() const {
    if(type_ != Type::MAP) {
        printf("The value type isn't Type::MAP \n");
        return nullptr;
    }
    return *field_.mapVal;
}

ValueMapIntKey& Value::asIntKeyMap()
{
    if(type_ != Type::INT_KEY_MAP) {
        printf("The value type isn't Type::INT_KEY_MAP \n");
        return nullptr;
    }
    return *field_.intKeyMapVal;
}

const ValueMapIntKey& Value::asIntKeyMap() const {
    if(type_ != Type::INT_KEY_MAP) {
        printf("The value type isn't Type::INT_KEY_MAP \n");
        return nullptr;
    }
    return *field_.intKeyMapVal;
}

static std::string getTabs(int depth) {
    std::string tabWidth;
    for (int i = 0; i < depth; ++i) {
        tabWidth += "\t";
    }
    return tabWidth;
}

static std::string visit(const Value& v, int depth);

static std::string visitVector(const ValueVector& v, int depth) {
    std::stringstream ret;

    if (depth > 0)
        ret << "\n";

    ret << getTabs(depth) << "[\n";

    int i = 0;
    for (const auto& child : v)
    {
        ret << getTabs(depth+1) << i << ": " << visit(child, depth + 1);
        ++i;
    }

    ret << getTabs(depth) << "]\n";

    return ret.str();
}

template <class T>
static std::string visitMap(const T& v, int depth) {
    std::stringstream ret;

    if (depth > 0)
        ret << "\n";

    ret << getTabs(depth) << "{\n";

    for (auto& iter : v)
    {
        ret << getTabs(depth + 1) << iter.first << ": ";
        ret << visit(iter.second, depth + 1);
    }

    ret << getTabs(depth) << "}\n";

    return ret.str();
}

static std::string visit(const Value& v, int depth) {
    std::stringstream ret;

    switch (v.getType()) {
        case Value::Type::NONE:
        case Value::Type::BYTE:
        case Value::Type::INTEGER:
        case Value::Type::UNSIGNED:
        case Value::Type::FLOAT:
        case Value::Type::DOUBLE:
        case Value::Type::BOOLEAN:
        case Value::Type::STRING:
            ret << v.asString() << "\n";
            break;
        case Value::Type::VECTOR:
            ret << visitVector(v.asValueVector(), depth);
            break;
        case Value::Type::MAP:
            ret << visitMap(v.asValueMap(), depth);
            break;
        case Value::Type::INT_KEY_MAP:
            ret << visitMap(v.asIntKeyMap(), depth);
            break;
        default:
            CCASSERT(false, "Invalid type!");
            break;
    }

    return ret.str();
}

std::string Value::getDescription() const {
    std::string ret("\n");
    ret += visit(*this, 0);
    return ret;
}

void Value::clear() {
    switch (type_)
    {
        case Type::BYTE:
            field_.byteVal = 0;
            break;
        case Type::INTEGER:
            field_.intVal = 0;
            break;
        case Type::UNSIGNED:
            field_.unsignedVal = 0u;
            break;
        case Type::FLOAT:
            field_.floatVal = 0.0f;
            break;
        case Type::DOUBLE:
            field_.doubleVal = 0.0;
            break;
        case Type::BOOLEAN:
            field_.boolVal = false;
            break;
        case Type::STRING:
            CC_SAFE_DELETE(field_.strVal);
            break;
        case Type::VECTOR:
            CC_SAFE_DELETE(field_.vectorVal);
            break;
        case Type::MAP:
            CC_SAFE_DELETE(field_.mapVal);
            break;
        case Type::INT_KEY_MAP:
            CC_SAFE_DELETE(field_.intKeyMapVal);
            break;
        default:
            break;
    }

    type_ = Type::NONE;
}

void Value::reset(Type type) {
    if (type_ == type) {
        return;
    }
    clear();

    switch (type) {
        case Type::STRING:
            field_.strVal = new (std::nothrow) std::string();
            break;
        case Type::VECTOR:
            field_.vectorVal = new (std::nothrow) ValueVector();
            break;
        case Type::MAP:
            field_.mapVal = new (std::nothrow) ValueMap();
            break;
        case Type::INT_KEY_MAP:
            field_.intKeyMapVal = new (std::nothrow) ValueMapIntKey();
            break;
        default:
            break;
    }

    type_ = type;
}

APRIL_END

