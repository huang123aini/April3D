#cmake 版本为 3.10
cmake_minimum_required(VERSION 3.10)

project(April3D)

#设置当前目录为引擎根目录
set(APRIL_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

#设置CMake模块目录
set(CMAKE_MODULE_PATH ${APRIL_ROOT_PATH}/cmake/Modules/)

#在编译库 libapril3d之前生效
include(${CMAKE_MODULE_PATH}/AprilBuildConfig)


option(USE_SOLUTION_FOLDERS "使用资源管理器文件夹" ON)

if(USE_SOLUTION_FOLDERS)
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
  set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMakeTargets")
endif()



add_subdirectory(${APRIL_ROOT_PATH}/april ${ENGINE_BINARY_PATH}/april/core)


set(BUILD_ENGINE_DONE ON)