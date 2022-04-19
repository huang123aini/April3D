
#pragma once

#include "platform/AprilPlatformConfig.h"
#include "platform/AprilPlatformDefine.h"

/**
 *引擎宏定义
 */
#define APRIL_BEGIN                     namespace april3d {
#define APRIL_END                      }


#define SAFE_DELETE(p)           do { delete (p); (p) = nullptr; } while(0)
#define SAFE_DELETE_ARRAY(p)     do { if(p) { delete[] (p); (p) = nullptr; } } while(0)
#define SAFE_FREE(p)             do { if(p) { free(p); (p) = nullptr; } } while(0)
#define SAFE_RELEASE(p)          do { if(p) { (p)->release(); } } while(0)
#define SAFE_RELEASE_NULL(p)     do { if(p) { (p)->release(); (p) = nullptr; } } while(0)
#define SAFE_RETAIN(p)           do { if(p) { (p)->retain(); } } while(0)
#define BREAK_IF(cond)           if(cond) break


#define CREATE_FUNC(__TYPE__) \
static __TYPE__* create() \
{ \
    __TYPE__ *pRet = new(std::nothrow) __TYPE__(); \
    if (pRet && pRet->init()) \
    { \
        pRet->autorelease(); \
        return pRet; \
    } \
    else \
    { \
        delete pRet; \
        pRet = nullptr; \
        return nullptr; \
    } \
}


#define NODE_FUNC(__TYPE__) \
DEPRECATED_ATTRIBUTE static __TYPE__* node() \
{ \
    __TYPE__ *pRet = new(std::nothrow) __TYPE__(); \
    if (pRet && pRet->init()) \
    { \
        pRet->autorelease(); \
        return pRet; \
    } \
    else \
    { \
        delete pRet; \
        pRet = NULL; \
        return NULL; \
    } \
}

#if (APRIL_OS == APRIL_OS_ANDROID)
    #define ENABLE_CACHE_TEXTURE_DATA       1
#else
    #define ENABLE_CACHE_TEXTURE_DATA       0
#endif

#if (APRIL_OS == APRIL_OS_ANDROID) || (APRIL_OS == APRIL_OS_WIN32)
    /** Application will crash in glDrawElements function on some win32 computers and some android devices.
     *  Indices should be bound again while drawing to avoid this bug.
     */
    #define REBIND_INDICES_BUFFER  1
#else
    #define REBIND_INDICES_BUFFER  0
#endif

   

#define PROPERTY_READONLY(varType, varName, funName)\
protected: varType varName; public: virtual varType get##funName() const;

#define PROPERTY_READONLY_PASS_BY_REF(varType, varName, funName)\
protected: varType varName; public: virtual const varType& get##funName() const;


#define PROPERTY(varType, varName, funName)\
protected: varType varName; public: virtual varType get##funName() const; virtual void set##funName(varType var);

#define PROPERTY_PASS_BY_REF(varType, varName, funName)\
protected: varType varName; public: virtual const varType& get##funName() const; virtual void set##funName(const varType& var);


#define SYNTHESIZE_READONLY(varType, varName, funName)\
protected: varType varName; public: virtual inline varType get##funName() const { return varName; }

#define SYNTHESIZE_READONLY_PASS_BY_REF(varType, varName, funName)\
protected: varType varName; public: virtual inline const varType& get##funName() const { return varName; }


#define SYNTHESIZE(varType, varName, funName)\
protected: varType varName; public: virtual inline varType get##funName() const { return varName; } virtual inline void set##funName(varType var){ varName = var; }

#define SYNTHESIZE_PASS_BY_REF(varType, varName, funName)\
protected: varType varName; public: virtual inline const varType& get##funName() const { return varName; } virtual inline void set##funName(const varType& var){ varName = var; }

#define SYNTHESIZE_RETAIN(varType, varName, funName)    \
private: varType varName; public: virtual inline varType get##funName() const { return varName; } virtual inline void set##funName(varType var) \
{ \
    if (varName != var) \
    { \
        SAFE_RETAIN(var); \
        SAFE_RELEASE(varName); \
        varName = var; \
    } \
} 



#define __LOGWITHFUNCTION(s, ...) \
    april3d::log("%s : %s",__FUNCTION__, april3d::StringUtils::format(s, ##__VA_ARGS__).c_str())

#if !defined(APRIL3D_DEBUG) || APRIL3D_DEBUG == 0
#define APRILLOG(...)       do {} while (0)
#define APRILLOGINFO(...)   do {} while (0)
#define APRILLOGERROR(...)  do {} while (0)
#define APRILLOGWARN(...)   do {} while (0)

#elif APRIL3D_DEBUG == 1
#define APRILLOG(format, ...)      april3d::log(format, ##__VA_ARGS__)
#define APRILLOGERROR(format,...)  april3d::log(format, ##__VA_ARGS__)
#define APRILLOGINFO(format,...)   do {} while (0)
#define APRILLOGWARN(...) __LOGWITHFUNCTION(__VA_ARGS__)

#elif APRIL3D_DEBUG > 1
#define APRILLOG(format, ...)      april3d::log(format, ##__VA_ARGS__)
#define APRILLOGERROR(format,...)  april3d::log(format, ##__VA_ARGS__)
#define APRILLOGINFO(format,...)   april3d::log(format, ##__VA_ARGS__)
#define APRILLOGWARN(...) __LOGWITHFUNCTION(__VA_ARGS__)
#endif



#if defined(__GNUC__) && ((__GNUC__ >= 5) || ((__GNUG__ == 4) && (__GNUC_MINOR__ >= 4))) \
    || (defined(__clang__) && (__clang_major__ >= 3)) || (_MSC_VER >= 1800)
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName &) = delete; \
    TypeName &operator =(const TypeName &) = delete;
#else
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName &); \
    TypeName &operator =(const TypeName &);
#endif


#define DISALLOW_IMPLICIT_CONSTRUCTORS(TypeName)    \
    TypeName();                                        \
    DISALLOW_COPY_AND_ASSIGN(TypeName)

#if defined(__GNUC__) && ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1)))
    #define DEPRECATED_ATTRIBUTE __attribute__((deprecated))
#elif _MSC_VER >= 1400 //vs 2005 or higher
    #define DEPRECATED_ATTRIBUTE __declspec(deprecated)
#else
    #define DEPRECATED_ATTRIBUTE
#endif 

#define DEPRECATED(...) DEPRECATED_ATTRIBUTE

#if defined(__GNUC__) && (__GNUC__ >= 4)
#define FORMAT_PRINTF(formatPos, argPos) __attribute__((__format__(printf, formatPos, argPos)))
#elif defined(__has_attribute)
  #if __has_attribute(format)
  #define FORMAT_PRINTF(formatPos, argPos) __attribute__((__format__(printf, formatPos, argPos)))
  #endif // __has_attribute(format)
#else
#define FORMAT_PRINTF(formatPos, argPos)
#endif

#if defined(_MSC_VER)
#define FORMAT_PRINTF_SIZE_T "%08lX"
#else
#define FORMAT_PRINTF_SIZE_T "%08zX"
#endif

#ifdef __GNUC__
#define UNUSED __attribute__ ((unused))
#else
#define UNUSED
#endif


#if !defined(REQUIRES_NULL_TERMINATION)
    #if defined(__APPLE_CC__) && (__APPLE_CC__ >= 5549)
        #define REQUIRES_NULL_TERMINATION __attribute__((sentinel(0,1)))
    #elif defined(__GNUC__)
        #define REQUIRES_NULL_TERMINATION __attribute__((sentinel))
    #else
        #define REQUIRES_NULL_TERMINATION
    #endif
#endif
