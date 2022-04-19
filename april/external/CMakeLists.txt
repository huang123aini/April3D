#添加对第三方库的管理

cmake_minimum_required(VERSION 3.6)
project(external)

set(APRIL_EXTERNAL_HEADER
  "AprilExternal.h"
)

set(APRIL_EXTERNAL_SRC
  "AprilExternal.cpp"
)

add_library(external ${APRIL_EXTERNAL_HEADER} ${APRIL_EXTERNAL_SRC})
