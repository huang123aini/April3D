#pragma once

#include "platform/AprilPlatformConfig.h"

/**
 *@定义每个平台的差异
 */ 
#if APRIL_OS == APRIL_OS_MAC
#include "platform/mac/april_os_mac.h"
#elif APRIL_OS == APRIL_OS_IOS
#include "platform/ios/april_os_ios.h"
#elif APRIL_OS == APRIL_OS_ANDROID
#include "platform/android/april_os_android.h"
#elif APRIL_OS == APRIL_OS_WIN32
#include "platform/win32/april_os_win32.h"
#elif APRIL_OS == APRIL_OS_LINUX
#include "platform/linux/april_os_linux.h"
#endif


