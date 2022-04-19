 #根据每个平台宏添加对应平台的代码
 
cmake_minimum_required(VERSION 3.10)
 
 if(IOS)
        set(APRIL_PLATFORM_SPECIFIC_HEADER
            platform/ios/april_os_ios.h
            platform/ios/gles_ios.h
            )
        set(APRIL_PLATFORM_SPECIFIC_SRC
           #添加.mm or .cpp
            )
endif()


if(ANDROID) 
# 添加android相关

endif()

if(MACOSX) 
#添加MAC相关

endif()

if(LINUX) 
#添加Linux相关

endif()

if(WINDOWS) 
#添加Windows相关

endif()


set(APRIL_PLATFORM_HEADER
    ${APRIL_PLATFORM_SPECIFIC_HEADER}
    platform/AprilGL.h
    platform/AprilPlatformConfig.h
    platform/AprilPlatformDefine.h
    platform/AprilPlatformMacros.h
    platform/FileUtils.h
    platform/AdjustBuffer.h
    platform/Image.h
    )

set(APRIL_PLATFORM_SRC
    ${APRIL_PLATFORM_SPECIFIC_SRC}
      platform/FileUtils.cpp
      platform/Image.cpp
    )
