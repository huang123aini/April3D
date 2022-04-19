#pragma once

#include "platform/AprilPlatformConfig.h"

/**
 * 每个平台的opengl es差异化
 */ 

#if (APRIL_OS == APRIL_OS_IOS)
#include "platform/ios/gles_ios.h"
#elif (APRIL_OS == APRIL_OS_ANDROID)
#include "platform/android/gles_android.h"
#elif (APRIL_OS == APRIL_OS_MAC)
#include "platform/mac/gl_mac.h"
#elif APRIL_OS == APRIL_OS_WIN32
#include "platform/win32/gl_win32.h"
#elif APRIL_OS == APRIL_OS_LINUX
#include "platform/linux/gl-linux.h"
#endif
