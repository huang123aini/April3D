cmake_minimum_required(VERSION 3.10)

set(APRIL_ENGINE_VERSION 1.0)

set(APRIL_CORE_LIB april3d)
#设置当前库
project(april3d_libs)

#使用标准的c++11
set(CMAKE_CXX_STANDARD 11)

#基础模块
include(base/CMakeLists.txt)
#平台相关模块
include(platform/CMakeLists.txt)
#绘制相关模块
include(renderer/CMakeLists.txt)
#数学模块
include(math/CMakeLists.txt)
#场景模块
include(scene/CMakeLists.txt)

#是否编译扩展功能
option(BUILD_EXTENSIONS "Build extension library" ON)

if(BUILD_EXTENSIONS)
   include(../extensions/CMakeLists.txt)
endif(BUILD_EXTENSIONS)


########################################添加头文件#######################################
set(APRIL_HEADER
    #对外暴露的头文件 
    april3d.h
    #基础模块
    ${APRIL_BASE_HEADER}
    #平台相关
    ${APRIL_PLATFORM_HEADER}
    #绘制模块
    ${APRIL_RENDERER_HEADER}
    #扩展功能模块
    #${APRIL_EXTENSIONS_HEADER}
    #数学模块
    ${APRIL_MATH_HEADER}
    #场景模块
    ${APRIL_SCENE_HEADER}
    )

######################################添加源文件########################################
set(APRIL_SRC 
    april3d.cpp
    ${APRIL_BASE_SRC}
    ${APRIL_PLATFORM_SRC}
    ${APRIL_RENDERER_SRC}
    ${APRIL_MATH_SRC}
    ${APRIL_SCENE_SRC}
    )

######################################################################################

add_library(april3d ${APRIL_HEADER} ${APRIL_SRC})


#添加平台依赖库
######################################################
if(IOS)

message(STATUS,"add ios locate system libraries start.")
# Locate system libraries on iOS
find_library(UIKIT_LIBRARY UIKit)
find_library(OPENGLES_LIBRARY OpenGLES)
find_library(CORE_MOTION_LIBRARY CoreMotion)
find_library(AVKIT_LIBRARY AVKit)
find_library(CORE_MEDIA_LIBRARY CoreMedia)
find_library(CORE_TEXT_LIBRARY CoreText)
find_library(SECURITY_LIBRARY Security)
find_library(CORE_GRAPHICS_LIBRARY CoreGraphics)
find_library(AV_FOUNDATION_LIBRARY AVFoundation)
find_library(WEBKIT_LIBRARY WebKit)
list(APPEND PLATFORM_SPECIFIC_LIBS
            ${UIKIT_LIBRARY}
            ${OPENGLES_LIBRARY}
            ${CORE_MOTION_LIBRARY}
            ${AVKIT_LIBRARY}
            ${CORE_MEDIA_LIBRARY}
            ${CORE_TEXT_LIBRARY}
            ${SECURITY_LIBRARY}
            ${CORE_GRAPHICS_LIBRARY}
            ${AV_FOUNDATION_LIBRARY}
            ${WEBKIT_LIBRARY}
            ${COCOS_APPLE_LIBS}
            "/usr/lib/libz.dylib"
            "/usr/lib/libiconv.dylib"
    )

#遍历并添加到链接库
foreach(platform_lib ${PLATFORM_SPECIFIC_LIBS})
  message(STATUS, ${platform_lib})
  target_link_libraries(april3d ${platform_lib})
endforeach()

message(STATUS,"add ios locate system libraries end.")
endif()

#######################################################

#使用第三方
#add_subdirectory(${APRIL_ROOT_PATH}/external ${ENGINE_BINARY_PATH}/external)
#target_link_libraries(april3d external)    


target_include_directories(april3d
   # PUBLIC ${APRIL_ROOT_PATH}
    PUBLIC ${APRIL_ROOT_PATH}/april
    #PUBLIC ${APRIL_ROOT_PATH}/extensions
    #PUBLIC ${APRIL_ROOT_PATH}/april/platform
    #PUBLIC ${APRIL_ROOT_PATH}/cocos/editor-support
    #INTERFACE ${APRIL_ROOT_PATH}/cocos/base
    #INTERFACE ${APRIL_ROOT_PATH}/cocos/audio/include
   INTERFACE ${APRIL_ROOT_PATH}/april/platform/${PLATFORM_FOLDER}
)

set_target_properties(april3d
                      PROPERTIES
                      ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                      LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                      VERSION "${APRIL_ENGINE_VERSION}"
                      FOLDER "Internal"
                      )




function(source_group_single_file single_file)
    set(oneValueArgs GROUP_TO BASE_PATH)
    cmake_parse_arguments(opt "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    # get relative_path
    get_filename_component(abs_path ${single_file} ABSOLUTE)
    file(RELATIVE_PATH relative_path_with_name ${opt_BASE_PATH} ${abs_path})
    get_filename_component(relative_path ${relative_path_with_name} PATH)
    # set source_group, consider sub source group
    string(REPLACE "/" "\\" ide_file_group "${opt_GROUP_TO}/${relative_path}")
    source_group("${ide_file_group}" FILES ${single_file})
endfunction()


#保持文件夹目录
if(XCODE OR VS)
    #cocos_mark_code_files("april3d")
     set(cocos_target "april3d")

     message(STATUS, "hsp hsp hsp")

     set(oneValueArgs GROUPBASE)
    cmake_parse_arguments(opt "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    if(NOT opt_GROUPBASE)
        set(root_dir ${CMAKE_CURRENT_SOURCE_DIR})
    else()
        set(root_dir ${opt_GROUPBASE})
        message(STATUS "target ${cocos_target} code group base is: ${root_dir}")
    endif()

    message(STATUS "cocos_mark_code_files: ${cocos_target}")

    get_property(file_list TARGET ${cocos_target} PROPERTY SOURCES)

    foreach(single_file ${file_list})
       source_group_single_file(${single_file} GROUP_TO "Source Files" BASE_PATH "${root_dir}")
    endforeach()

endif()
